


void LoadSprites() {
	LoadSprite(&Data->playerSprite, "data/ship_player.png");
	LoadSprite(&Data->enemySprite, "data/player_guy.png");

	LoadSprite(&Data->lemonSprite, "data/bad_lemon.png");

	LoadSprite(&Data->playerBulletSprite, "data/bullet_blue.png");
	LoadSprite(&Data->playerLaserChargeSprite, "data/laserBall.png");
	LoadSprite(&Data->playerChargedLaserShotSprite, "data/laserBullet.png");

}

void LoadSoundClips() {
	LoadSoundClip("data/sfx/deep_laser.wav", &Data->soundDeepLaser);
}