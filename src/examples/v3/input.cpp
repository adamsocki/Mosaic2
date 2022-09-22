


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



void PlayerShootSpaceBarController(Player* p, bool spaceHeld, int32 inputTimer) {
	
	if (InputHeldSeconds(Input, Input_Space, inputTimer)) {
		if (!spaceHeld) {
			EntityHandle playerLaserChargeHandle = AddEntity(&Data->em, EntityType_PlayerLaserCharge);
			PlayerLaserCharge* playerLaserChargeEntity = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
			p->currentLaserChargeHandle = playerLaserChargeHandle;
			playerLaserChargeEntity->toDelete = false;
			playerLaserChargeEntity->position = p->position;
			playerLaserChargeEntity->size = V2(0.5f, 0.5f);
			playerLaserChargeEntity->strength = 0.002f;
			playerLaserChargeEntity->growthRate = 1;
			playerLaserChargeEntity->sprite = &Data->playerLaserChargeSprite;
			spaceHeld = true;
			p->chargingLaser = true;
		}
		else {
			EntityHandle playerLaserChargeHandle = p->currentLaserChargeHandle;
			PlayerLaserCharge* playerLaserChargeEntity = (PlayerLaserCharge*)GetEntity(&Data->em, playerLaserChargeHandle);
			playerLaserChargeEntity->strength += Game->deltaTime * playerLaserChargeEntity->growthRate;
			if (playerLaserChargeEntity->size.x < 3) {
				playerLaserChargeEntity->size.x += playerLaserChargeEntity->strength;
				playerLaserChargeEntity->size.y += playerLaserChargeEntity->strength;

			}
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