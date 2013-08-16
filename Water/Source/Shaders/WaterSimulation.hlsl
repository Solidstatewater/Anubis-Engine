//hold all data about water column
struct WaterColumn
{
	float	m_height;
	float4	m_flow;
};

//keep all grid in structured buffers
RWStructuredBuffer<WaterColumn> NextWaterState		:	register( u0 );
StructuredBuffer<WaterColumn>	CurrentWaterState	:	register( t0 );

//parameters
cbuffer UpdateParameters	:	register( c0 )
{
	float4	timer;
	float4	dispatchSize;
};

#define size_x	16
#define size_y	16

#define padded_x (1 + size_x + 1)
#define padded_y (1 + size_y + 1)

//put all data for calculations to the group shared memory
groupshared WaterColumn columns[padded_x * padded_y];

//one thread for each column
[numthreads(padded_x, padded_y, 1)]
void WaterCS(uint3 GroupId : SV_GroupID, uint3 DispatchThreadId : SV_DispathThreadID,
			 uint3 GroupThreadId : SV_GroupThreadID, uint GroupIndex : SV_GroupIndex)
{
	///////////////////////////////////////
	//Initializing
	///////////////////////////////////////

	//load size of the whole grid
	int2 grid_size;
	grid_size.x = dispatchSize.x;
	grid_size.y = dispatchSize.y;

	/*** Calculate index in the grid ***/
	int3 index = int3(0, 0, 0);
	index.x = GroupId.x * size_x + (GroupThreadId.x - 1);
	index.y = GroupId.y * size_y + (GroupThreadId.y - 1);
	index.z = 0;

	uint texture_index = index.x + index.y * grid_size.x;

	//load the data
	columns[GroupIndex] = CurrentWaterState[texture_index];

	//make sure each thread has loaded data to the group shared memory
	GroupMemoryBarrierWithGroupSync();


	////////////////////////////////////////
	//Calculation
	////////////////////////////////////////

	float4 flow = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//make sure it's not right edge
	if (GroupThreadId.x < (padded_x - 1) && index.x < (grid_size.x - 1))
	{
		flow.x = columns[GroupIndex+1].m_height - columns[GroupIndex].m_height;

		//make sure it's not bottom edge
		if (GroupThreadId.y < (padded_y - 1) && index.y < (grid_size.y - 1))
		{
			flow.y = columns[GroupIndex+1+padded_x].m_height - columns[GroupIndex].m_height;
		}
	}

	//make sure it's not bottom edge
	if (GroupThreadId.y < (padded_y - 1) && index.y < (grid_size.y - 1))
	{
		flow.z = columns[GroupIndex + padded_x].m_height - columns[GroupIndex].m_height;

		//make sure it's not left edge
		if (GroupThreadId.x > 0 && GroupThreadId.x > 0)
		{
			flow.w = columns[GroupIndex-1+padded_x].m_height - columns[GroupIndex].m_height;
		}
	}

	//constants
	const float TIME_STEP = 0.05f;
	const float PIPE_AREA = 0.0001f;
	const float GRAVITATION = 10.0f;
	const float PIPE_LENGTH = 0.2f;
	const float FLUID_DENSITY = 1.0f;
	const float COLUMN_AREA = 0.05f;
	const float DAMPING_FACTOR = 0.9995f;

	//calculate acceleration
	float acceleration = ( min(timer.x, TIME_STEP) * PIPE_AREA * GRAVITATION ) 
		/ (PIPE_LENGTH * COLUMN_AREA);

	//calculate flow
	flow = (flow * acceleration + columns[GroupIndex].m_flow) * DAMPING_FACTOR;

	//store the updated flow value
	columns[GroupIndex].m_flow = flow;

	//synchronize all threads
	GroupMemoryBarrierWithGroupSync();


	////////////////////////////////////////////
	//Update columns height values
	////////////////////////////////////////////

	//flows out of this column
	columns[GroupIndex].m_height =  columns[GroupIndex].m_height + flow.x +
									flow.y + flow.z + flow.w;

	//from top
	columns[GroupIndex].m_height =  columns[GroupIndex].m_height + 
									columns[GroupIndex-padded_x].m_flow.z;

	//from top-right
	columns[GroupIndex].m_height =	columns[GroupIndex].m_height +
									columns[GroupIndex-padded_x+1].m_flow.w;

	//from left
	columns[GroupIndex].m_height =	columns[GroupIndex].m_height +
									columns[GroupIndex-1].m_flow.x;

	//from left-top
	columns[GroupIndex].m_height =	columns[GroupIndex].m_height +
									columns[GroupIndex-padded_x-1].m_flow.y;


	//////////////////////////////////////////////
	//Finally, store all values in UAV
	//////////////////////////////////////////////

	//NOTE:DO NOT update values of perimeter threads!!!
	if (GroupThreadId.x > 0 && GroupThreadId.y > 0 &&
		GroupThreadId.x < (padded_x-1) && GroupThreadId.y < (padded_y-1))
	{
		NextWaterState[texture_index] = columns[GroupIndex];
	}
}