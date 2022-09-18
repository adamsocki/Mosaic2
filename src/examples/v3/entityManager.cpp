


MyData* Data = NULL;

int32 freeList[1000];
int32 freeListCount = 0;

void* GetEntity(EntityManager* em, EntityHandle handle) {

	if (handle.idLocationInsideInfo >= em->totalEntityManagerCapacity) {
		return NULL;
	}

	EntityInfo* info = &Data->em.entities[handle.idLocationInsideInfo];

	if (info->generation != handle.generation) {
		return NULL;
	}
	if (info->type != handle.type) {
		return NULL;
	}

	EntityTypeBuffer* buffer = &em->buffers[info->type];

	return ((u8*)buffer->entities + (buffer->sizeInBytes * info->indexInBuffer));
}

void DeleteEntity(EntityManager* em, EntityHandle handle) {
	
	freeList[freeListCount] = handle.idLocationInsideInfo;
	freeListCount++;

	EntityTypeBuffer* buffer = &em->buffers[handle.type];
	EntityInfo* info = &em->entities[handle.idLocationInsideInfo];
	EntityHandle handleOfSwappedEntity;

	if (handle.type == EntityType_PlayerBullet) {
		PlayerBullet* playerBulletEntity = (PlayerBullet*)buffer->entities;
		
		playerBulletEntity[info->indexInBuffer] = playerBulletEntity[buffer->count - 1];
		handleOfSwappedEntity = playerBulletEntity[buffer->count - 1].handle;
		
		EntityInfo* infoOfSwappedEntity = &em->entities[handleOfSwappedEntity.idLocationInsideInfo];
		infoOfSwappedEntity->indexInBuffer = info->indexInBuffer;
	}
	if (handle.type == EntityType_Enemy) {
		Enemy* enemyEntity = (Enemy*)buffer->entities;

		enemyEntity[info->indexInBuffer] = enemyEntity[buffer->count - 1];
		handleOfSwappedEntity = enemyEntity[buffer->count - 1].handle;

		EntityInfo* infoOfSwappedEntity = &em->entities[handleOfSwappedEntity.idLocationInsideInfo];
		infoOfSwappedEntity->indexInBuffer = info->indexInBuffer;
	}
	
	
	buffer->count--;
}

EntityHandle AddEntity(EntityManager *em, EntityType type) {
	
	int32 nextFreeIdInIndex = em->nextID;

	if (freeListCount > 0) {
		nextFreeIdInIndex = freeList[freeListCount - 1];
		freeListCount--;
	}
	else {
		em->nextID++;
	}

	EntityInfo* info = &em->entities[nextFreeIdInIndex];
	info->type = type;
	info->generation++;

	EntityTypeBuffer* buffer = &em->buffers[type];
	info->indexInBuffer = buffer->count;
	buffer->count++;

	EntityHandle handle = {};
	handle.generation = info->generation;
	handle.idLocationInsideInfo = nextFreeIdInIndex;
	handle.type = type;

	return handle;
}


void CreatePlayer() {
	EntityHandle playerHandle = AddEntity(&Data->em, EntityType_Player);
	Player* playerEntity = (Player*)GetEntity(&Data->em, playerHandle);
	playerEntity->handle = playerHandle;
	playerEntity->position = V2(1, 1);
	playerEntity->size = V2(0.7f, 0.3f);
	playerEntity->sprite = &Data->playerSprite;
	playerEntity->speed = 4;
	playerEntity->toDelete = false;
}


void InitializeEntityBuffers() {
	// PlayerBuffer
	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	playerBuffer->capacity = 1;
	playerBuffer->sizeInBytes = sizeof(Player);
	playerBuffer->count = 0;
	playerBuffer->entities = (Player*)malloc(playerBuffer->capacity * playerBuffer->sizeInBytes);

	// EnemyBuffer
	EntityTypeBuffer* enemyBuffer = &Data->em.buffers[EntityType_Enemy];
	enemyBuffer->capacity = 100;
	enemyBuffer->sizeInBytes = sizeof(Enemy);
	enemyBuffer->count = 0;
	enemyBuffer->entities = (Enemy*)malloc(enemyBuffer->capacity * enemyBuffer->sizeInBytes);

	// PlayerBulletBuffer
	EntityTypeBuffer* playerBulletBuffer = &Data->em.buffers[EntityType_PlayerBullet];
	playerBulletBuffer->capacity = 200;
	playerBulletBuffer->sizeInBytes = sizeof(PlayerBullet);
	playerBulletBuffer->count = 0;
	playerBulletBuffer->entities = (PlayerBullet*)malloc(playerBulletBuffer->capacity * playerBulletBuffer->sizeInBytes);

	// starBuffer
	EntityTypeBuffer* starBuffer = &Data->em.buffers[EntityType_Star];
	starBuffer->capacity = 500;
	starBuffer->sizeInBytes = sizeof(Star);
	starBuffer->count = 0;
	starBuffer->entities = (Star*)malloc(starBuffer->capacity * starBuffer->sizeInBytes);
}

void InitializeLevelManager() {
	Data->lm.spawnEnemyTimer = 0;
	Data->lm.spawnEnemyGenerations = 0;
	Data->lm.spawnEnemies = false;
	Data->lm.renderStarTimer = 0;
	Data->lm.renderStars = true;
}

void InitializeEntityManager() {

	// Entity Manager
	Data->em.totalEntityManagerCapacity = 1000;
	Data->em.entities = (EntityInfo*)malloc(sizeof(EntityInfo) * Data->em.totalEntityManagerCapacity);
	memset(Data->em.entities, 0, sizeof(EntityInfo) * Data->em.totalEntityManagerCapacity);
	Data->em.nextID = 0;
}


void CreateStars(int32 numberToCreate, bool playingGame) {
	
	for (int i = 0; i < numberToCreate; i++) {
		EntityHandle starHandle = AddEntity(&Data->em, EntityType_Star);
		Star* starEntity = (Star*)GetEntity(&Data->em, starHandle);
		starEntity->handle = starHandle;
		if (!playingGame) {
			starEntity->position = V2(RandfRange(-10, 20), RandfRange(-10, 10));
		}
		else {
			starEntity->position = V2(RandfRange(10, 11), RandfRange(-10, 10));
		}
		real32 starSize = RandfRange(.01f, 0.04f);
		starEntity->size = V2(starSize, starSize);
		starEntity->speed = V2(RandfRange(-5, -1), 0);
		starEntity->toDelete = false;
		starEntity->sprite = &Data->starSprite;
	}
}