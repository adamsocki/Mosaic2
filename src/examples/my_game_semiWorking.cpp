#include "my_game_structs.cpp"

#include "my_game_EntityManager.cpp"

#include "my_game_input.cpp"

#include "my_game_assets.cpp" // Sprites|Sounds




// MyInit()

#include "my_game_collision.cpp"

// MyUpdate()

#include "my_game_render.cpp"





void InitializeLevelManager() {
	
}

void MyInit() {
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	Data = (MyData*)Game->myData;

	LoadSprites();

	// Step 1: Initialize Entity Manager 
	InitializeEntityManager();
	//InitializeLevelManager();

	// Step 2: ADD PLAYER ENTITY
	EntityHandle playerHandle = AddEntity(&Data->em, EntityType_Player);

	// Step 3: GET PLAYER ENTITY
	Player* p = (Player*)GetEntity(&Data->em, playerHandle);
	// set player entity values
	p->position = V2(0, 0);
	p->speed = 4;
	p->handle = playerHandle;
	p->size = V2(0.5f, 0.27f);
	p->sprite = &Data->playerSprite;

	// Step 4: Set up Level Manager
	Data->lm.timer = 0;
}

real32 inputTimer = 1;
bool spaceHeld = false;

void MyGameUpdate() {

	// LOGIC
	//		LEVEL LOGIC
	Data->lm.timer += Game->deltaTime;

	real32 levelTimer = Data->lm.timer;
	if (levelTimer > 5) {
		Data->lm.createNewEnemies = true;
	}

	EntityTypeBuffer* playerBulletBuffer = &Data->em.buffers[EntityType_PlayerBullet];
	//int32* numBulletPointer = &playerBulletBuffer->count;
	PlayerBullet* pb = (PlayerBullet*)playerBulletBuffer->entities;

	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	int32 numOfPlayers = playerBuffer->count;
	Player* p = (Player*)playerBuffer->entities;
	
	EntityTypeBuffer* playerLaserChargeBuffer = &Data->em.buffers[EntityType_PlayerLaserCharge];
	int32 numOfPlayerLaserCharge = playerLaserChargeBuffer->count;
	PlayerLaserCharge* plc = (PlayerLaserCharge*)playerLaserChargeBuffer->entities;

	EntityTypeBuffer* playerChargedLaserShotBuffer = &Data->em.buffers[EntityType_PlayerChargedLaserShot];
	int32 numOfChargedLaserShot = playerChargedLaserShotBuffer->count;
	PlayerChargedLaserShot* pclShotGet = (PlayerChargedLaserShot*)playerChargedLaserShotBuffer->entities;

	EntityTypeBuffer* enemyBuffer = &Data->em.buffers[EntityType_Enemy];
	int32 numOfEnemies = enemyBuffer->count;
	Enemy* e = (Enemy*)enemyBuffer->entities;

	//		INPUT LOGIC
	PlayerInputMove(p);

	// SpaceBar Control Logic
	if (InputHeldSeconds(Input, Input_Space, inputTimer)) {
		// Spawn Player Laser Entity
		if (!spaceHeld) {
			EntityHandle playerLaserChargeHandle = AddEntity(&Data->em, EntityType_PlayerLaserCharge);
			PlayerLaserCharge* plcGet = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
			p->currentLaserChargeHandle = playerLaserChargeHandle;
			plcGet->toDelete = false;
			plcGet->position = p->position;
			plcGet->size = V2(0.5f, 0.5f);
			plcGet->strength = 0.002f;
			plcGet->sprite = &Data->playerLaserChargeSprite;
			plcGet->handle = playerLaserChargeHandle;
			spaceHeld = true;
			p->chargingLaser = true;
		}
		else {
			EntityHandle playerLaserChargeHandle = p->currentLaserChargeHandle;
			PlayerLaserCharge* plcGet = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
			plcGet->strength += Game->deltaTime * plcGet->growthRate;
			if (plcGet->size.x < 3) {
				plcGet->size.x += plcGet->strength;
				plcGet->size.y += plcGet->strength;
			}
		}
	}
	// What happens when you release the spacebar
	if (InputReleased(Input, Input_Space)) {
		if (spaceHeld) {
			// Create Charged Shot
			EntityHandle playerChargedLaserShotHandle = AddEntity(&Data->em, EntityType_PlayerChargedLaserShot);
			PlayerChargedLaserShot* pclShotGet = (PlayerChargedLaserShot*)GetEntity(&Data->em, playerChargedLaserShotHandle);
			
			EntityHandle playerLaserChargeHandle = AddEntity(&Data->em, EntityType_PlayerLaserCharge);
			PlayerLaserCharge* plcGet = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
			
			pclShotGet->power = plcGet->size.x;
			pclShotGet->position = plcGet->position;
			pclShotGet->speed = pclShotGet->power * 10;
			pclShotGet->sprite = &Data->playerChargedLaserShotSprite;
			pclShotGet->handle = playerChargedLaserShotHandle;
			p->currentChargedLaserShotHandle = playerChargedLaserShotHandle;

			spaceHeld = false;
			inputTimer = 1;
			p->chargingLaser = false;
			p->shootingLaser = true;
			//plcGet->toDelete = true;
		}
	}
	
	if (InputPressed(Input, Input_Space)) {
 		EntityHandle playerBulletHandle = AddEntity(&Data->em, EntityType_PlayerBullet);
		pb = (PlayerBullet*)GetEntity(&Data->em, playerBulletHandle);
		pb->lifetime = 0;
		pb->toDelete = false;
		pb->position = p->position;
		pb->sprite = &Data->playerBulletSprite;
		pb->speed = V2(6, 0);
		pb->size = V2(0.1f, 0.1f);
		pb->handle = playerBulletHandle;
		PlaySound(&Game->audioPlayer, Data->soundDeepLaser, 1.0f, true);
	}

	// Update Current LaserChargeHandle to match player position
	numOfPlayerLaserCharge = playerLaserChargeBuffer->count;
	if (p->chargingLaser) {
		for (int i = 0; i < numOfPlayerLaserCharge; i++) {
			EntityHandle playerLaserChargeHandle = p->currentLaserChargeHandle;
			PlayerLaserCharge* plcGet = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
			plcGet->position = p->position + V2(0, 2);
		}
	}

	// Move Laser Shot
	numOfChargedLaserShot = playerChargedLaserShotBuffer->count;
	if (p->shootingLaser) {
		for (int i = 0; i < numOfChargedLaserShot; i++) {

			EntityHandle playerChargedLaserShotHandle = p->currentChargedLaserShotHandle;
			PlayerChargedLaserShot* pclShotGet = (PlayerChargedLaserShot*)GetEntity(&Data->em, playerChargedLaserShotHandle);
			pclShotGet->position.x = p->position.x;
			pclShotGet->position.y += Game->deltaTime * pclShotGet->speed;
		}
	}
	
	//		PLAYER LOGIC
	//MouseDirection();
	
	//		PLAYER BULLET LOGIC
	//numBulletPointerint32 numOfPlayerBullets = playerBulletBuffer->count;
	//			Access Logic to All Bullets
	
 	for (int i = 0; i < playerBulletBuffer->count; i++) {
		EntityHandle playerBulletHandle = pb[i].handle;
		PlayerBullet* pb_entity = (PlayerBullet*)GetEntity(&Data->em, playerBulletHandle);
		if (pb_entity != NULL) {
			pb_entity->position.x += pb_entity->speed.x * Game->deltaTime;
			pb_entity->lifetime += Game->deltaTime;
			if (pb_entity->lifetime > 2) {
				pb_entity->toDelete = true;
			}
		}
	}


	//		ENEMY LOGIC
	

	if (Data->lm.createNewEnemies) {
		for (int i = 0; i < 5; i++) {
			EntityHandle enemyHandle = AddEntity(&Data->em, EntityType_Enemy);
			Enemy *e_create = (Enemy*)GetEntity(&Data->em, enemyHandle);
			e_create->toDelete = false;
			e_create->position = V2(6, -3 + i);
			e_create->sprite = &Data->enemySprite;
			e_create->speed = V2(-2, 0);
			e_create->handle = enemyHandle;
			e_create->size = V2(0.4f, 0.4f);
		}
		Data->lm.createNewEnemies = false;
		Data->lm.timer = 0;
	}

	numOfEnemies = enemyBuffer->count;
	//numOfPlayerBullets = playerBulletBuffer->count;
	//e = (Enemy*)enemyBuffer->entities;
	for (int i = 0; i < numOfEnemies; i++) {
		EntityHandle enemyHandle = e[i].handle;
		Enemy* e_entity = (Enemy*)GetEntity(&Data->em, enemyHandle);
		if (e_entity != NULL) {
			// Move Enemy
			e_entity->position.x += e_entity->speed.x * Game->deltaTime;
			// Detect Collision with Bullet
			for (int j = 0; j < playerBulletBuffer->count; j++) {
				EntityHandle playerBulletHandle = pb[j].handle;
				PlayerBullet* pb_entity = (PlayerBullet*)GetEntity(&Data->em, playerBulletHandle);
				if (pb_entity != NULL) {
					Circle enemyCircle;
					Circle bulletCircle;
					enemyCircle.center = e_entity->position;
					enemyCircle.radius = e_entity->size.x;
					bulletCircle.center = pb_entity->position;
					bulletCircle.radius = e_entity->size.x;
					real32 distanceBulletToEnemy = Distance(enemyCircle.center, bulletCircle.center);

					if (distanceBulletToEnemy <= (bulletCircle.radius + enemyCircle.radius)) {
  						pb_entity->toDelete = true;
						e_entity->toDelete = true;
						
					}
				}
				
			}
		}
	}




	// Collision Detection

		// BULLET TO ENEMEY
		

	//for (int i = 0; num)
	//CollisionDetection();

	
	// Step 1: Create a circle for entities
	


	
	// RENDER


	//RenderEntities();
	ClearColor(RGB(0.4f, 0.0f, 0.0f));

	//		RENDER BASE

	//		RENDER PLAYER

	// RENDER & DELETE
	//		RENDER/DELETE PLAYER
	for (int i = 0; i < numOfPlayers; i++) {
		DrawSprite(p->position, p->size, p->sprite);
	}

	//		RENDER ENEMY
	for (int i = 0; i < numOfEnemies; i++) {
		EntityHandle enemyHandle = e[i].handle;
		Enemy *e_entity = (Enemy*)GetEntity(&Data->em, enemyHandle);
		if (e_entity != NULL) {
			DrawSprite(e_entity->position, e_entity->size, e_entity->sprite);
		}
	
	}

	//		RENDER PLAYER BULLET
	for (int i = 0; i < playerBulletBuffer->count; i++) {
		EntityHandle playerBulletHandle = pb[i].handle;
		PlayerBullet* pb_entity = (PlayerBullet*)GetEntity(&Data->em, playerBulletHandle);
		if (pb_entity != NULL) {
			DrawSprite(pb_entity->position, V2(0.1f, 0.1f), pb_entity->sprite);
			//DrawTextScreenPixel(&Game->monoFont, V2(540, 251), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%f", pb_entity->position.x);
			//DrawTextScreenPixel(&Game->monoFont, V2(540, 281), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%f", pb_entity->position.y);
		}
		
	}
	
	//		RENDER PLAYER LASER CHARGE
	if (p->chargingLaser) {
		for (int i = 0; i < numOfPlayerLaserCharge; i++) {
			EntityHandle playerLaserChargeHandle = plc[i].handle;
			PlayerLaserCharge* plc_entity = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
			if (plc_entity != NULL) {
				DrawSprite(plc_entity->position, plc_entity->size, plc_entity->sprite);

			}
		}
	}
	
	//		RENDER PLAYER LASER SHOT
	if (p->shootingLaser) {
		for (int i = 0; i < numOfChargedLaserShot; i++) {
			EntityHandle playerLaserChargeHandle = pclShotGet[i].handle;
			PlayerChargedLaserShot* pclShot_entity = (PlayerChargedLaserShot*)GetEntity(&Data->em, playerLaserChargeHandle);
			if (pclShot_entity != NULL) {
				DrawSprite(pclShot_entity->position, V2(1,1), pclShot_entity->sprite);

			}
		}
	}

	for (int i = 0; i < playerBulletBuffer->count; i++) {
		EntityHandle playerBulletHandle = pb[i].handle;
		PlayerBullet* pb_entity = (PlayerBullet*)GetEntity(&Data->em, playerBulletHandle);
		if (pb_entity != NULL) {
			if (pb_entity->toDelete) {
				DeleteEntity(&Data->em, pb_entity->handle);
			}
		}

	}

	for (int i = 0; i < numOfEnemies; i++) {
		EntityHandle enemyHandle = e[i].handle;
		Enemy* e_entity = (Enemy*)GetEntity(&Data->em, enemyHandle);
		if (e_entity != NULL) {
			if (e_entity->toDelete) {
				DeleteEntity(&Data->em, e_entity->handle);
			}
			
		}

	}

	EntityHandle playerLaserChargeHandle = p->currentLaserChargeHandle;
	PlayerLaserCharge* plcGet = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
	if (plcGet != NULL) {
		if (plcGet->toDelete) {
			DeleteEntity(&Data->em, plcGet->handle);
		}
	}
}