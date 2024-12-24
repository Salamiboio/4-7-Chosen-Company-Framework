class CHO_RespawnMenuOpen: SCR_InventoryAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
 	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		
		GetGame().GetMenuManager().OpenDialog(ChimeraMenuPreset.CHO_RespawnMenu);
 	}
	
	override bool CanBeShownScript(IEntity user)
	{
		CharacterControllerComponent charComp = CharacterControllerComponent.Cast(user.FindComponent(CharacterControllerComponent));
		return charComp.GetInspect();
	}
}