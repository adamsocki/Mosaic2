//#include "my_game_structs.cpp"



void PlayerInputMove(Player* p) {
	vec2 slowDown = V2(0,0);
	//real32 slowDown = 1;
	//bool = twoDirections;
	
	
	if (InputHeld(Input, Input_Up)) {
		slowDown.y = 1;
		//p->position.y += p->speed * Game->deltaTime;
		if (InputHeld(Input, Input_Right)) {
			slowDown.x = 1;
		}
		if (InputHeld(Input, Input_Left)) {
			slowDown.x = -1;
		}
	}
	
	if (InputHeld(Input, Input_Down)) {
		slowDown.y = -1;
		//p->position.y -= p->speed * Game->deltaTime;
		if (InputHeld(Input, Input_Right)) {
			slowDown.x = 1;
		}
		if (InputHeld(Input, Input_Left)) {
			slowDown.x = -1;
		}
	}
	if (InputHeld(Input, Input_Right)) {
		slowDown.x = 1;
	}
	if (InputHeld(Input, Input_Left)) {
		slowDown.x = -1;
	}

	/*
	if (InputHeld(Input, Input_LeftArrow)) {
		if (InputHeld(Input, Input_DownArrow)) {
			p->position.y -= p->speed * Game->deltaTime;
		}
		
		if (InputHeld(Input, Input_UpArrow) || InputHeld(Input, Input_DownArrow)) {
			slowDown = p->speed / 8;
		}
		p->position.x -= p->speed * Game->deltaTime * slowDown;
	}


	if (InputHeld(Input, Input_RightArrow)) {
		if (InputHeld(Input, Input_UpArrow) || InputHeld(Input, Input_DownArrow)) {
			slowDown = p->speed / 8;
		}
		p->position.x += p->speed * Game->deltaTime * slowDown;
	}
	*/
	// Move Player
	p->position.x += p->speed * slowDown.x * Game->deltaTime;
	p->position.y += p->speed * slowDown.y * Game->deltaTime;

}

