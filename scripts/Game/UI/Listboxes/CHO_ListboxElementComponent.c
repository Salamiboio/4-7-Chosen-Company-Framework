class CHO_ListBoxElementComponent: SCR_ListBoxElementComponent
{
	int m_iSlotPlayerID;
	void SetPlayerName(string input)
	{
		TextWidget.Cast(m_wRoot.FindAnyWidget("PlayerName")).SetText(input);
	}
	
	void SetStatus(bool input)
	{
		if(!input)
			ImageWidget.Cast(m_wRoot.FindAnyWidget("HealthImage")).SetVisible(true);
		else
		{
			ImageWidget.Cast(m_wRoot.FindAnyWidget("DeathImage")).SetVisible(true);
			ImageWidget.Cast(m_wRoot.FindAnyWidget("Underline")).SetColor(Color.FromRGBA(208,0,0,255));
		}
	}
	
	SCR_ButtonTextComponent GetRespawnButton()
	{
		return SCR_ButtonTextComponent.Cast(m_wRoot.FindAnyWidget("RespawnButton").FindHandler(SCR_ButtonTextComponent));
	}
}