
enum EntityType {
	EntityType_Base,
	EntityType_Star,

	EntityType_Player,
	EntityType_Enemy,

	EntityType_PlayerBullet,
	EntityType_EnemyBullet,

	EntityType_PlayerChargedLaserShot,
	EntityType_PlayerLaserCharge,

	EntityType_Count,
};


struct EntityInfo {
	int32 indexInBuffer;
	int32 generation;

	EntityType type;
};

struct EntityHandle {
	int32 idLocationInsideInfo;
	int32 generation;

	EntityType type;
};

struct EntityTypeBuffer {
	int32 count;
	int32 capacity;
	int32 sizeInBytes;

	void* entities;
};

struct EntityManager {
	int32 totalEntityManagerCapacity;
	int32 nextID;

	EntityTypeBuffer buffers[EntityType_Count];
	EntityInfo* entities;
};


struct Entity {
	vec2 position;
	vec2 size;

	Sprite* sprite;

	bool toDelete;

	EntityHandle handle;
};


struct Player : Entity {
	real32 speed;
	bool chargingLaser;
	bool shootingLaser;

	EntityHandle currentLaserChargeHandle;
};

struct PlayerBullet : Entity {

	real32 lifetime;
	vec2 speed;

};


struct Enemy : Entity {
	vec2 speed;
	real32 lifetime;
};

struct Star : Entity {
	vec2 speed;
};

struct EnemyBullet : Entity {
	vec2 speed;
};

struct PlayerLaserCharge : Entity {
	real32 strength;
	real32 growthRate;
};

struct PlayerChargedLaserShot : Entity {

};


struct LevelManager {
	bool renderStars;
	real32 renderStarTimer;

	real32 spawnEnemyTimer;
	int32 spawnEnemyGenerations;
	bool spawnEnemies;
};

struct MyData {

	EntityManager em;

	Sprite playerSprite;
	//Sprite enemySprite;
	Sprite playerBulletSprite;

	Sprite enemySprite1;
	Sprite enemySprite2;
	Sprite enemySprite3;
	Sprite enemySprite4;
	Sprite enemySprite5;
	Sprite enemySprite6;
	Sprite enemySprite7;
	Sprite enemySprite8;

	Sprite playerLaserChargeSprite;

	Sprite starSprite;

	LevelManager lm;
};