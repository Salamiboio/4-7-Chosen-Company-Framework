modded class Bacon_628B48364AFD286C_EditableIntelEntity
{
	[Attribute("")]
	protected string m_sInitialString;
	
	override void EOnInit(IEntity owner) {
		super.EOnInit(owner);
		
		if(Replication.IsServer())
			SetIntel(m_sInitialString);
	}
}