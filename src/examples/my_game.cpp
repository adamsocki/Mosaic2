
#include "v3/structs.cpp"
#include "v3/entityManager.cpp"
#include "v3/loadSprites.cpp"
#include "v3/input.cpp"
#include "v3/entityController.cpp"


void MyInit() {
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	Data = (MyData*)Game->myData;


	Data->spacebarHeld = false;
	Data->inputTimer = 1;
	Data->killCount = 0;
	// v3/structs.cpp
	LoadSprites();

	// v3/entityManager.cpp
	InitializeEntityManager();

	InitializeEntityBuffers();

	InitializeLevelManager();
	CreateStars(120, false);

	CreatePlayer();	
	CreateBase();
}


void MyGameUpdate() {


	// LOGIC
	//		Level Logic
	Data->lm.spawnEnemyTimer += Game->deltaTime;
	if (Data->lm.spawnEnemyTimer > 2.5f) {
		Data->lm.spawnEnemyTimer = 0;
		Data->lm.spawnEnemies = true;
		Data->lm.spawnEnemyGenerations++;
	}

	//		Point to EntityBuffers
	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	EntityTypeBuffer* enemyBuffer = &Data->em.buffers[EntityType_Enemy];
	EntityTypeBuffer* playerBulletBuffer = &Data->em.buffers[EntityType_PlayerBullet];
	EntityTypeBuffer* starBuffer = &Data->em.buffers[EntityType_Star];
	EntityTypeBuffer* playerLaserChargeBuffer = &Data->em.buffers[EntityType_PlayerLaserCharge];
	EntityTypeBuffer* playerLaserShotBuffer = &Data->em.buffers[EntityType_PlayerLaserShot];
	EntityTypeBuffer* baseBuffer = &Data->em.buffers[EntityType_Base];

	//		Point to EntityBuffer->entities
	Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;
	Enemy* enemyEntitiesInBuffer = (Enemy*)enemyBuffer->entities;
	PlayerBullet* playerBulletEntitiesInBuffer = (PlayerBullet*)playerBulletBuffer->entities;
	Star* starEntitiesInBuffer = (Star*)starBuffer->entities;
	PlayerLaserCharge* playerLaserChargeEntitiesInBuffer = (PlayerLaserCharge*)playerLaserChargeBuffer->entities;
	PlayerLaserShot* playerLaserShotEntitiesInBuffer = (PlayerLaserShot*)playerLaserShotBuffer->entities;
	Base* baseEntitiesInBuffer = (Base*)baseBuffer->entities;

	//		Input Logic
	InputPlayerController(&playerEntitiesInBuffer[0]);
	PlayerShootSpaceBarController(&playerEntitiesInBuffer[0]);

	//		Spawn Enemies - Level Timer Check
	if (Data->lm.spawnEnemies) {
		for (int i = 0; i < 6; i++) {
			EntityHandle enemyHandle = AddEntity(&Data->em, EntityType_Enemy);
			Enemy* enemyEntity = (Enemy*)GetEntity(&Data->em, enemyHandle);
			enemyEntity->position = V2(7, -4 + i * RandfRange(0.1f, 3) );
			enemyEntity->size = V2(0.2f, 0.2f);
			enemyEntity->handle = enemyHandle;
			enemyEntity->toDelete = false;
			enemyEntity->speed = V2(RandfRange(-3, -0.5f), 0);
			int32 random = RandiRange(1, 8);
			if (random == 1) {
				enemyEntity->sprite = &Data->enemySprite1;
			}
			else if (random == 2) {
				enemyEntity->sprite = &Data->enemySprite2;
			}
			else if (random == 3) {
				enemyEntity->sprite = &Data->enemySprite3;
			}
			else if (random == 4) {
				enemyEntity->sprite = &Data->enemySprite4;
			}
			else if (random == 5) {
				enemyEntity->sprite = &Data->enemySprite5;
			}
			else if (random == 6) {
				enemyEntity->sprite = &Data->enemySprite6;
			}
			else if (random == 7) {
				enemyEntity->sprite = &Data->enemySprite7;
			}
			else if (random == 8) {
				enemyEntity->sprite = &Data->enemySprite8;
			}
			enemyEntity->lifetime = 4;
		}
		Data->lm.spawnEnemies = false;
	}

	//		Move Enemies
	for (int i = 0; i < enemyBuffer->count; i++) {
		EntityHandle enemyHandle = enemyEntitiesInBuffer[i].handle;
		Enemy* enemyEntity = (Enemy*)GetEntity(&Data->em, enemyHandle);
		if (enemyEntity != NULL) {
			enemyEntity->position.x += enemyEntity->speed.x * Game->deltaTime;
			enemyEntity->lifetime += Game->deltaTime;
		}
	}

	//		Move Player Laser
	if (playerEntitiesInBuffer[0].shootingLaser) {
		for (int i = 0; i < playerLaserShotBuffer->count; i++) {
			PlayerLaserShot* playerLaserShotEntity = (PlayerLaserShot*)GetEntity(&Data->em, playerEntitiesInBuffer[0].playerLaserShotHandle);
			playerLaserShotEntity->position = playerEntitiesInBuffer[0].position + V2(1+(playerLaserShotEntity->size.x), 0);
			playerLaserShotEntity->lifetime += Game->deltaTime;

			if (playerLaserShotEntity->lifetime > 2) {
				playerLaserShotEntity->toDelete = true;
			}

			// Collision detection to enemy
			Rect playerLaserShotRect;
			playerLaserShotRect.max = playerLaserShotEntity->size - V2(0, 0.5f);
			playerLaserShotRect.min = -playerLaserShotEntity->size + V2(0, 0.5f);

			for (int j = 0; j < enemyBuffer->count; j++) {
				EntityHandle enemyHandle = enemyEntitiesInBuffer[j].handle;
				Enemy* enemyEntity = (Enemy*)GetEntity(&Data->em, enemyHandle);
				if (enemyEntity != NULL) {
					Rect enemyRect;
					enemyRect.max = enemyEntity->size;
					enemyRect.min = -enemyEntity->size;
					vec2 dir = V2(0);
					if (RectTest(enemyRect, playerLaserShotRect, enemyEntity->position, playerLaserShotEntity->position, &dir)) {
						//playerLaserShotEntity->toDelete = true;
						enemyEntity->toDelete = true;
						Data->killCount++;
					}
				}
			}

		}
	}

	//		Move Player Bullets
	for (int i = 0; i < playerBulletBuffer->count; i++) {
		
		EntityHandle playerBulletHandle = playerBulletEntitiesInBuffer[i].handle;
		PlayerBullet* playerBulletEntity = (PlayerBullet*)GetEntity(&Data->em, playerBulletHandle);

		if (playerBulletEntity != NULL) {
			Rect playerBulletRect;
			playerBulletRect.max = playerBulletEntity->size;
			playerBulletRect.min = -playerBulletEntity->size;	
			//Movement of bullet
			playerBulletEntity->position.x += playerBulletEntity->speed.x * Game->deltaTime;
			playerBulletEntity->lifetime += Game->deltaTime;
			if (playerBulletEntity->lifetime > 2) {
				playerBulletEntity->toDelete = true;
			}
			//		Collision Detection to Enemy
			for (int j = 0; j < enemyBuffer->count; j++) {
				EntityHandle enemyHandle = enemyEntitiesInBuffer[j].handle;
				Enemy* enemyEntity = (Enemy*)GetEntity(&Data->em, enemyHandle);
				if (enemyEntity != NULL) {
					Rect enemyRect;
					enemyRect.max = enemyEntity->size - V2(0.1f,0.1f);
					enemyRect.min = -enemyEntity->size + V2(0.1f, 0.1f);
					vec2 dir = V2(0);
					if (RectTest(enemyRect, playerBulletRect, enemyEntity->position, playerBulletEntity->position, &dir)) {
						playerBulletEntity->toDelete = true;
						enemyEntity->toDelete = true;
						Data->killCount++;
					}
				}
			}
		}
	}

	//		Create new stars
	CreateStars(121 - starBuffer->count, false);
	
	

	// RENDER & DELETE
	ClearColor(RGB(0.0f, 0.0f, 0.0f));



	//		Star Render (andMover)
	for (int i = 0; i < starBuffer->count; i++) {
		EntityHandle starHandle = starEntitiesInBuffer[i].handle;
		Star* starEntity = (Star*)GetEntity(&Data->em, starHandle);
		if (starEntity != NULL) {
			starEntity->position.x += Game->deltaTime * starEntity->speed.x;
			if (starEntity->position.x < -10) {
				starEntity->toDelete = true;
			}
		}
	}



	for (int i = 0; i < starBuffer->count; i++) {
		EntityHandle starHandle = starEntitiesInBuffer[i].handle;
		Star* starEntity = (Star*)GetEntity(&Data->em, starHandle);
		if (starEntity != NULL) {
			
			if (starEntity->toDelete) {
				DeleteEntity(&Data->em, starEntity->handle);
			}
			else {
				DrawSprite(starEntity->position, starEntity->size, starEntity->sprite);
			}
			
		}
	}


	//		Player Render
	for (int i = 0; i < playerBuffer->count; i++) {
		EntityHandle playerHandle = playerEntitiesInBuffer[i].handle;
		Player* playerEntity = (Player*)GetEntity(&Data->em, playerHandle);
		if (playerEntity != NULL) {
			DrawSprite(playerEntity->position, playerEntity->size, playerEntity->sprite);
		}
	}
	//		Enemy Redner
	for (int i = 0; i < enemyBuffer->count; i++) {
 		EntityHandle enemyHandle = enemyEntitiesInBuffer[i].handle;
		Enemy* enemyEntity = (Enemy*)GetEntity(&Data->em, enemyHandle);
		if (enemyEntity != NULL) {
			if (enemyEntity->toDelete) {
				DeleteEntity(&Data->em, enemyEntity->handle);
			}
			else {
				DrawSprite(enemyEntity->position, enemyEntity->size, enemyEntity->sprite);
			}
		}
		
	}
	//		Player Bullet Render & Delete
	for (int i = 0; i < playerBulletBuffer->count; i++) {
		EntityHandle playerBulletHandle = playerBulletEntitiesInBuffer[i].handle;
		PlayerBullet* playerBulletEntity = (PlayerBullet*)GetEntity(&Data->em, playerBulletHandle);
		if (playerBulletEntity != NULL) {
			if (playerBulletEntity->toDelete) {
				DeleteEntity(&Data->em, playerBulletEntity->handle);
			}
			else {
				DrawSprite(playerBulletEntity->position, playerBulletEntity->size, playerBulletEntity->sprite);
			}
		}
	} 

	if (playerEntitiesInBuffer[0].chargingLaser) {
		for (int i = 0; i < playerLaserChargeBuffer->count; i++) {
			EntityHandle playerLaserChargeHandle = playerLaserChargeEntitiesInBuffer[i].handle;
			PlayerLaserCharge* playerLaserChargeEntity = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
			if (playerLaserChargeEntity != NULL) {
				if (playerLaserChargeEntity->toDelete) {
					DeleteEntity(&Data->em, playerLaserChargeEntity->handle);
				}
				else {
					DrawTextScreenPixel(&Game->monoFont, V2(30, 80), 13.0f, RGB(0.1f, 0.1f, 0.99f), "Laser Charge: %2f", playerLaserChargeEntity->size.x / 3);
					DrawSprite(playerLaserChargeEntity->position, playerLaserChargeEntity->size, playerLaserChargeEntity->sprite);
				}
			}
		}
	}

	if (playerEntitiesInBuffer[0].shootingLaser) {
		for (int i = 0; i < playerLaserShotBuffer->count; i++) {
			EntityHandle playerLaserShotHandle = playerLaserShotEntitiesInBuffer[i].handle;
			PlayerLaserCharge* playerLaserShotEntity = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserShotHandle);
			if (playerLaserShotEntity != NULL) {
				if (playerLaserShotEntity->toDelete) {
					DeleteEntity(&Data->em, playerLaserShotEntity->handle);
				}
				else {
					DrawSprite(playerLaserShotEntity->position, playerLaserShotEntity->size, playerLaserShotEntity->sprite);
				}
				
			}
		}
	}

	DrawTextScreenPixel(&Game->monoFont, V2(30,50), 13.0f, RGB(0.8f, 0.1f, 0.1f), "Kill Count: %d", Data->killCount);
	
}
