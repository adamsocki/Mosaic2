/*
	vec2 mousePos = Input->mousePosNormSigned;
	//real32 val;

	//real32 dotval = Dot(mouseNormal, V2(0, 0));

	//val = atan2l(mouseNormal.y, mouseNormal.x);
	real32 anglePlayerToMouse;
	anglePlayerToMouse = Angle(V2(0, 0), mousePos);

	vec2 playerPos = p[0].position;

	real32 angle1 = Angle(mousePos, playerPos);
	mousePos.x = mousePos.x * 8;
	mousePos.y = mousePos.y * 4.5f;
	//real32 lengthMouse = Length(mousePos);


	vec2 newVector = V2( mousePos.x - playerPos.x, mousePos.y - playerPos.y );


	real32 arcTan = atanf(newVector.x / newVector.y);

//	real32 distance = Distance(p[0].position, mousePos);

	DrawSprite(mousePos, V2(0.5f, 0.5f), &Data->lemonSprite);


	*/
	/*
	real32 lengthPlayer = Length(p[0].position);

	real32 dot = Dot(mousePos, p[0].position);

	real32 acos = acosf(dot);
	real32 val = acosf(dot) / (lengthPlayer * lengthMouse);

	vec2 mouseNormal;
	mouseNormal = Normalize(mousePos);


	vec2 playerPosNormal;
	playerPosNormal = Normalize(p[0].position);


	real32 angle = Angle(mouseNormal, V2(0, 0));


	real32 cross = Cross(mousePos, p[0].position);
	*/

	//real32 arcCos = acosf(mouseNormal.x);
	//real32 x = mousePos.x - p[0].position.x;
	//real32 y = mousePos.y - p[0].position.y;
	/*
	vec2 test = Normalize(V2(x, y));
	*/
	//real32 val = atan2l(mousePos, playerPos);
	/*
	real32 mouseLength = Length(mousePos);
	real32 playerLength = Length(playerPos);

	real32 dot = Dot(mousePos, playerPos);

	real32 value = dot / (mouseLength * playerLength);

	real32 arcCos = acosf(value);

	real32 angle2 = Angle(mousePos, playerPos);


	DrawTextScreenPixel(&Game->monoFont, V2(540, 251), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%f", mouseLength);
	DrawTextScreenPixel(&Game->monoFont, V2(540, 281), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%f", playerLength);
	DrawTextScreenPixel(&Game->monoFont, V2(540, 311), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%f", dot);
	DrawTextScreenPixel(&Game->monoFont, V2(540, 341), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%f", value);
	DrawTextScreenPixel(&Game->monoFont, V2(540, 371), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%f", arcCos);
	DrawTextScreenPixel(&Game->monoFont, V2(540, 401), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%f", angle1);
	DrawTextScreenPixel(&Game->monoFont, V2(540, 431), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%f", angle2);

	*/

	//handle_weMoved
	//info(handle_weMoved.id) 

	//info->indexInBuffer = buffer->count - 1;
	// get an entity pointer to point to

	//buffer->entities[info->indexInBuffer] = buffer->entities[buffer->count - 1];

	// get and modify the index for the object obtained from INFO 
	//buffer->entities + (buffer->entitySizeByte * info->indexInBuffer) = buffer->entities + (buffer->entitySizeByte * buffer->count - 1);

	//((u8*)buffer->entities + (buffer->entitySizeByte * info->indexInBuffer));

	/*
	pb[i].position.y += (pb[i].speed.y * Game->deltaTime);
	pb->toDelete = false;
	pb[i].toDelete = false;
	pb[i].lifetime += Game->deltaTime;
	if (pb[i].lifetime > 1) {
		pb[i].toDelete = true;
	}
	*/


	/*
	// get the entity buffer allocated with the handle
	Entity* e = (Entity *)buffer->entities;
	e[info->indexInBuffer].handle = handle;
	//ffer.entities[nextFreeIndex].handle = handle;
	*/
	// generate the handle
	// Add the Generation from info to the handle ???
	//handle.generation = info->generation;
	/*
		//TEST CODE FOR DELETE AND ADD
		if (InputPressed(Input, Input_Space)) {
			//Pass in the entity's handle that we want to delete.
			if (numOfEnemies != 0) {
				DeleteEntity(&Data->em, &e[3].handle);
			}
		}
		if (InputPressed(Input, Input_UpArrow)) {
			EntityHandle enemyHandle = AddEntity(&Data->em, EntityType_Enemy);
			Enemy* e = (Enemy*)GetEntity(&Data->em, enemyHandle);
			e->position = V2(1, 4);
			e->sprite = &Data->enemySprite;
		}
		*/
		// Step 4: Test Deletion of Enemy
			/*
			for (int i = 0; i < 8; i++) {
				EntityHandle enemyHandle = AddEntity(&Data->em, EntityType_Enemy);
				Enemy* e = (Enemy*)GetEntity(&Data->em, enemyHandle);
				e->position = V2(1, i);
				e->handle = enemyHandle;
				e->sprite = &Data->enemySprite;
			}
			*/
