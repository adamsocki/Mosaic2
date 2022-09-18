


MyData* Data = NULL;

int32 freeList[100];
int32 freeListCount = 0;
real32 angle = 0;



void DeleteEntity(EntityManager* em, EntityHandle handle) {
	freeList[freeListCount] = handle.id;
	freeListCount++;
	// grab BUFFER for type
	EntityTypeBuffer* buffer = &em->buffers[handle.type];

	// grab INFO for position of enitity
	EntityInfo* info = &em->entities[handle.id];

	// increase generation in the entities handle
	//handle->generation++;

	// try to increase generation in the info index
	//info->generation++;
	if (handle.type == EntityType_PlayerBullet) {
		PlayerBullet* pb = (PlayerBullet*)buffer->entities;
		pb[info->indexInBuffer] = pb[buffer->count - 1];
		EntityHandle handle_weMoved = pb[buffer->count - 1].handle;
		EntityInfo* info_weMoved = &em->entities[handle_weMoved.id];
		info_weMoved->indexInBuffer = info->indexInBuffer;
	}
	else if (handle.type == EntityType_Enemy) {
		Enemy* e = (Enemy*)buffer->entities;
		e[info->indexInBuffer] = e[buffer->count - 1];
		EntityHandle handle_weMoved = e[buffer->count - 1].handle;
		EntityInfo* info_weMoved = &em->entities[handle_weMoved.id];
		info_weMoved->indexInBuffer = info->indexInBuffer;
	}
	
	buffer->count--;
}

EntityHandle AddEntity(EntityManager* em, EntityType type) {

	// NEED TO DO FREELIST HERE
	// calculate which index is coming up next that is free
	int32 nextFreeIndex = em->nextID;
	//		check to see if there are any free values that we can utilize
	//		these will only arrise when we have deleted some entity
	if (freeListCount > 0) {
		nextFreeIndex = freeList[freeListCount - 1];
		freeListCount--;
	}
	else {
		em->nextID++;
	}

	// Create handle for function's return and to be placed into the Entity within the buffer
	EntityHandle handle = {};

	// Find a new ID to add that will create a new EntityInfo
	EntityInfo* info = &em->entities[nextFreeIndex];

	// Add the type we are adding to the info's type
	info->type = type;
	info->generation++;

	// Fetch the Buffer using the type which contains our entities for each entity type
	EntityTypeBuffer* buffer = &em->buffers[type];

	// use the next count that is available in that buffer for the info that is being used
	info->indexInBuffer = buffer->count;

	// increase the buffer's count
	buffer->count++;


	handle.generation = info->generation;
	// Add the buffer's nextID to the handle
	handle.id = nextFreeIndex;
	// add the type to the handle
	handle.type = type;
	return handle;

}

void* GetEntity(EntityManager* em, EntityHandle handle) {
	// use the HANDLE to find the entity INFO

	// make sure there are sufficent entity capacities avaialable
	if (handle.id >= em->entityCapacity) {
		return NULL;
	}

	//use the handle to retrive the info
	EntityInfo* info = &em->entities[handle.id];

	// make sure that the generation's match
	if (info->generation != handle.generation) {
		return NULL;
	}
	if (info->type != handle.type) {
		return NULL;
	}

	//retrieve the buffer that we are looking for by type
	EntityTypeBuffer* buffer = &em->buffers[info->type];
	// find the place for that specific buffer in the memory using the index in the buffer that is in the EntityInfo
	return ((u8*)buffer->entities + (buffer->entitySizeByte * info->indexInBuffer));

}

void InitializeEntityManager() {

	// Step 1: SET UP ENTITY MANAGER
	Data->em.entityCapacity = 2000;
	// allocate memory for EntityManger
	// the structure of entities is based on the EntityInfo structure
	Data->em.entities = (EntityInfo*)malloc(sizeof(EntityInfo) * Data->em.entityCapacity);
	// initialize to zero ???
	memset(Data->em.entities, 0, sizeof(EntityInfo) * Data->em.entityCapacity);
	// Step 1: Set our NextID counter to 0;
	Data->em.nextID = 0;

	// Step 2: CREATE PLAYER BUFFER
	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	// Determine the size of the Entity
	playerBuffer->entitySizeByte = sizeof(Player);
	// How many can be stored in here (will use for allocation below)
	playerBuffer->capacity = 10;
	// Set the counter equal to 0 so it starts at the beginning
	playerBuffer->count = 0;
	// Allocate the memory for the entities within the buffer 
	playerBuffer->entities = (Player*)malloc(playerBuffer->entitySizeByte * playerBuffer->capacity);
	memset(playerBuffer->entities, 0, sizeof(EntityTypeBuffer) * playerBuffer->capacity);

	// Step 3: CREATE BASE BUFFER


	// Step 4: CREATE ENEMY BUFFER
	EntityTypeBuffer* enemyBuffer = &Data->em.buffers[EntityType_Enemy];
	// Determine the size of the Entity
	enemyBuffer->entitySizeByte = sizeof(Enemy);
	// How many can be stored in here (will use for allocation below)
	enemyBuffer->capacity = 1000;
	// Set the counter equal to 0 so it starts at the beginning
	enemyBuffer->count = 0;
	// Allocate the memory for the entities within the buffer
	enemyBuffer->entities = (Enemy*)malloc(enemyBuffer->entitySizeByte * enemyBuffer->capacity);
	//memset(playerBuffer->entities, 0, sizeof(EntityTypeBuffer) * playerBuffer->capacity);
	memset(enemyBuffer->entities, 0, sizeof(EntityTypeBuffer) * enemyBuffer->capacity);


	// Step 5: CREATE PLAYER BULLET BUFFER
	EntityTypeBuffer* playerBulletBuffer = &Data->em.buffers[EntityType_PlayerBullet];
	// Determine the size of the Entity
	playerBulletBuffer->entitySizeByte = sizeof(PlayerBullet);
	// How many can be stored in here (will use for allocation below)
	playerBulletBuffer->capacity = 200;
	// Set the counter equal to 0 so it starts at the beginning
	playerBulletBuffer->count = 0;
	// Allocate the memory for the entities within the buffer
	playerBulletBuffer->entities = (PlayerBullet*)malloc(playerBulletBuffer->entitySizeByte * playerBulletBuffer->capacity);
	memset(playerBulletBuffer->entities, 0, sizeof(EntityTypeBuffer) * playerBulletBuffer->capacity);

	// Step 6: CREATE PLAYER LASER CHARGE BUFFER
	EntityTypeBuffer* playerLaserChargeBuffer = &Data->em.buffers[EntityType_PlayerLaserCharge];
	// Determine the size of the Entity
	playerLaserChargeBuffer->entitySizeByte = sizeof(PlayerLaserCharge);
	// How many can be stored in here (will use for allocation below)
	playerLaserChargeBuffer->capacity = 10;
	// Set the counter equal to 0 so it starts at the beginning
	playerLaserChargeBuffer->count = 0;
	// Allocate the memory for the entities within the buffer
	playerLaserChargeBuffer->entities = (PlayerLaserCharge*)malloc(playerLaserChargeBuffer->entitySizeByte * playerLaserChargeBuffer->capacity);
	memset(playerLaserChargeBuffer->entities, 0, sizeof(EntityTypeBuffer) * playerLaserChargeBuffer->capacity);


	// Step 7: CREATE PLAYER CHARGED LASER SHOT BUFFER
	EntityTypeBuffer* playerChargedLaserShotBuffer = &Data->em.buffers[EntityType_PlayerChargedLaserShot];
	// Determine the size of the Entity
	playerChargedLaserShotBuffer->entitySizeByte = sizeof(PlayerChargedLaserShot);
	// How many can be stored in here (will use for allocation below)
	playerChargedLaserShotBuffer->capacity = 10;
	// Set the counter equal to 0 so it starts at the beginning
	playerChargedLaserShotBuffer->count = 0;
	// Allocate the memory for the entities within the buffer
	playerChargedLaserShotBuffer->entities = (PlayerChargedLaserShot*)malloc(playerChargedLaserShotBuffer->entitySizeByte * playerChargedLaserShotBuffer->capacity);
	memset(playerChargedLaserShotBuffer->entities, 0, sizeof(EntityTypeBuffer) * playerChargedLaserShotBuffer->capacity);

}