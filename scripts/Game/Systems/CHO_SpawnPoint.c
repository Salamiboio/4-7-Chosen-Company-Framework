class CHO_SpawnPointClass: SCR_BaseGameModeComponentClass
{

}

class CHO_SpawnPoint: SCR_BaseGameModeComponent
{
	[Attribute("50", desc: "Distance around the OriginalSpawn trigger we will teleport players.")]
	protected int m_iSpawnDistance;
	
	override void OnControllableSpawned(IEntity entity)
	{
		super.OnControllableSpawned(entity);
		IEntity originalSpawn = GetGame().GetWorld().FindEntityByName("OriginalSpawn");
		if(CLB_PlayableCharacter.Cast(entity.FindComponent(CLB_PlayableCharacter)).IsPlayable() && vector.Distance(entity.GetOrigin(), originalSpawn.GetOrigin()) < m_iSpawnDistance)
			GetGame().GetCallqueue().CallLater(MoveEntity, 500, false, entity);
	}
	
	protected void MoveEntity(IEntity entity)
	{
		if(!GetGame())
			return;

		if(!GetGame().GetWorld())
			return;
		
		vector newSpawn[4]; 
		GetGame().GetWorld().FindEntityByName("NewSpawn").GetWorldTransform(newSpawn);
		BaseGameEntity baseGameEntity = BaseGameEntity.Cast(entity); 
		vector originalPosition[4];
		baseGameEntity.GetWorldTransform(originalPosition);
		vector originalSpawnPosition[4];
		GetGame().GetWorld().FindEntityByName("OriginalSpawn").GetWorldTransform(originalSpawnPosition);
		vector finalPosition[4];
		finalPosition = newSpawn;
		finalPosition[3][0] = finalPosition[3][0] + (originalPosition[3][0] - originalSpawnPosition[3][0]);
		finalPosition[3][2] = finalPosition[3][2] + (originalPosition[3][2] - originalSpawnPosition[3][2]);
		
		if(!baseGameEntity)
			return;

		float scale = baseGameEntity.GetScale();
		baseGameEntity.Teleport(finalPosition);
        baseGameEntity.SetWorldTransform(finalPosition);
        baseGameEntity.SetScale(scale);
        baseGameEntity.Update();
        baseGameEntity.OnTransformReset();

		Physics phys = baseGameEntity.GetPhysics();
		if (phys)
		{
			phys.SetVelocity(vector.Zero);
			phys.SetAngularVelocity(vector.Zero);
		}
	}
}