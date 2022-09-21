


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

real32 inputTimer = 1;
bool spaceHeld = false;

void PlayerShootSpaceBarController(Player* p) {
	

	
	
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