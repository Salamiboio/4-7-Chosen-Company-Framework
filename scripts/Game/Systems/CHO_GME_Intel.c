modded class GME_IntelComponent
{
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		if(Replication.IsServer())
			OnContentChanged();
	}
}