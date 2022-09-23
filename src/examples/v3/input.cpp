


void InputPlayerController(Player* p) {

	vec2 speedController = V2(0, 0);

	if (InputHeld(Input, Input_Up)) {
		speedController.y = 1;
		if (InputHeld(Input, Input_Right)) {
			speedController.x = 1;
		}
		if (InputHeld(Input, Input_Left)) {
			speedController.x = -1;
		}
	}
	if (InputHeld(Input, Input_Down)) {
		speedController.y = -1;
		if (InputHeld(Input, Input_Right)) {
			speedController.x = 1;
		}
		if (InputHeld(Input, Input_Left)) {
			speedController.x = -1;
		}
	}
	if (InputHeld(Input, Input_Right)) {
		speedController.x = 1;
	}
	if (InputHeld(Input, Input_Left)) {
		speedController.x = -1;
	}
	p->position.x += p->speed * speedController.x * Game->deltaTime;
	p->position.y += p->speed * speedController.y * Game->deltaTime;
}



void PlayerShootSpaceBarController(Player* p) {
	
	if (InputHeldSeconds(Input, Input_Space, Data->inputTimer)) {
		if (!Data->spacebarHeld) {
			EntityHandle playerLaserChargeHandle = AddEntity(&Data->em, EntityType_PlayerLaserCharge);
			PlayerLaserCharge* playerLaserChargeEntity = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
			vec2 laserChargePositionRelativeToPlayer = p->position;
			laserChargePositionRelativeToPlayer.x += 2;
			playerLaserChargeEntity->position = laserChargePositionRelativeToPlayer;
			playerLaserChargeEntity->handle = playerLaserChargeHandle;
			playerLaserChargeEntity->strength = 0.002f;
			playerLaserChargeEntity->sprite = &Data->playerLaserChargeSprite;
			playerLaserChargeEntity->size = V2(0.5f, 0.5f);
			playerLaserChargeEntity->growthRate = 0.001f;
			playerLaserChargeEntity->toDelete = false;
			Data->spacebarHeld = true;
			p->chargingLaser = true;
			p->chargingLaserHandle = playerLaserChargeHandle;
		}
		else {
			PlayerLaserCharge* playerLaserChargeEntity = (PlayerLaserCharge*)GetEntity(&Data->em, p->chargingLaserHandle);
			playerLaserChargeEntity->strength += Game->deltaTime * playerLaserChargeEntity->growthRate;
			vec2 laserChargePositionRelativeToPlayer = p->position;
			laserChargePositionRelativeToPlayer.x += 2;
			playerLaserChargeEntity->position = laserChargePositionRelativeToPlayer;
			if (playerLaserChargeEntity->size.x < 3) {
				playerLaserChargeEntity->size.x += playerLaserChargeEntity->strength;
				playerLaserChargeEntity->size.y += playerLaserChargeEntity->strength;
			}
		}
		
	}

	if (InputReleased(Input, Input_Space)) {
		if (Data->spacebarHeld) {
			PlayerLaserCharge* playerLaserChargeEntity = (PlayerLaserCharge*)GetEntity(&Data->em, p->chargingLaserHandle);
			playerLaserChargeEntity->toDelete = true;
			Data->spacebarHeld = false;
			p->chargingLaser = false;

			// create laser
			p->shootingLaser = true;
			EntityHandle playerLaserShotHandle = AddEntity(&Data->em, EntityType_PlayerLaserShot);
			PlayerLaserShot* playerLaserShotEntity = (PlayerLaserShot*)GetEntity(&Data->em, playerLaserShotHandle);

			playerLaserShotEntity->size = V2(11, 0.5f);
			playerLaserShotEntity->position = p->position + V2(1 + (playerLaserShotEntity->size.x/2 ), 0);
			playerLaserShotEntity->sprite = &Data->playerLaserShotSprite;
			playerLaserShotEntity->lifetime = 0;
			playerLaserShotEntity->toDelete = false;
			playerLaserShotEntity->handle = playerLaserShotHandle;
			p->playerLaserShotHandle = playerLaserShotHandle;
		}		
	}
		
	if (InputPressed(Input, Input_Space)) {
		EntityHandle playerBulletHandle = AddEntity(&Data->em, EntityType_PlayerBullet);
		PlayerBullet* playerBulletEntity = (PlayerBullet*)GetEntity(&Data->em, playerBulletHandle);
		playerBulletEntity->lifetime = 0;
		playerBulletEntity->toDelete = false;
		playerBulletEntity->position = p->position;
		playerBulletEntity->sprite = &Data->playerBulletSprite;
		playerBulletEntity->speed = V2(6, 0);
		playerBulletEntity->size = V2(0.1f, 0.1f);
		playerBulletEntity->handle = playerBulletHandle;
	}
}