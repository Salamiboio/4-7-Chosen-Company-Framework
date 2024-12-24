modded enum ChimeraMenuPreset
{
	CHO_RespawnMenu
}

class CHO_RespawnMenuUI: ChimeraMenuBase
{
	protected CRF_Gamemode m_Gamemode;
	protected Widget m_wRoot;
	protected CHO_ListboxComponent m_wPlayerList;
	protected TextWidget m_wAliveText;
	protected bool m_bGroupToggle = true;
	protected int localSlotChanges = 0;
	
	override void OnMenuOpen()
	{	
		if (RplSession.Mode() == RplMode.Dedicated) {
			Close();
			return;
		}
		
		GetGame().GetInputManager().AddActionListener("MenuBack", EActionTrigger.DOWN, Action_Exit);
		
		m_wRoot = GetRootWidget();
		m_Gamemode = CRF_Gamemode.Cast(GetGame().GetGameMode());
		
		m_wPlayerList = CHO_ListboxComponent.Cast(m_wRoot.FindAnyWidget("PlayersList").FindHandler(CHO_ListboxComponent));
		m_wAliveText = TextWidget.Cast(m_wRoot.FindAnyWidget("AliveText"));
		localSlotChanges = m_Gamemode.m_iSlotChanges;
		UpdateSlots();
		
		SCR_ButtonTextComponent.Cast(m_wRoot.FindAnyWidget("ToggleButton").FindHandler(SCR_ButtonTextComponent)).m_OnClicked.Insert(ToggleGroup);
		SCR_ButtonTextComponent.Cast(m_wRoot.FindAnyWidget("RespawnAll").FindHandler(SCR_ButtonTextComponent)).m_OnClicked.Insert(RespawnAll);
	}
	
	void ToggleGroup()
	{
		m_bGroupToggle = !m_bGroupToggle;
		UpdateSlots();
	}
	
	void RespawnAll()
	{
		ref array<int> playerIDs = {};
		GetGame().GetPlayerManager().GetAllPlayers(playerIDs);
		foreach(int playerID: playerIDs)
		{
			if(!m_Gamemode.m_aEntityDeathStatus.Get(m_Gamemode.m_aSlots.Find(playerID)))
				continue;
			
			if(m_Gamemode.m_aSlots.Find(playerID) != -1)
			{
				int index = m_Gamemode.m_aSlots.Find(playerID);
				string respawnPlayerPrefab = m_Gamemode.m_aSlotPrefabs.Get(index);
				vector position = SCR_PlayerController.GetLocalControlledEntity().GetOrigin();
				int groupID = SCR_AIGroup.Cast(RplComponent.Cast(Replication.FindItem(m_Gamemode.m_aActivePlayerGroupsIDs.Get(m_Gamemode.m_aGroupRplIDs.Find(m_Gamemode.m_aPlayerGroupIDs.Get(index))))).GetEntity()).GetGroupID();
				SCR_PlayerController.Cast(GetGame().GetPlayerController()).Respawn(playerID, respawnPlayerPrefab, position, groupID);
			}
			else
			{
				string respawnPlayerPrefab = "{577C074AAF8AE737}Prefabs/Characters/Factions/BLUFOR/Chosen Company/Character_Chosen_Rifleman.et";
				vector position = SCR_PlayerController.GetLocalControlledEntity().GetOrigin();
				int groupID = SCR_GroupsManagerComponent.GetInstance().GetPlayerGroup(SCR_PlayerController.GetLocalPlayerId()).GetGroupID();
				SCR_PlayerController.Cast(GetGame().GetPlayerController()).Respawn(playerID, respawnPlayerPrefab, position, groupID);
			}
		}
	}
	
	void Respawn(int playerID)
	{
		if(m_Gamemode.m_aSlots.Find(playerID) != -1)
		{
			int index = m_Gamemode.m_aSlots.Find(playerID);
			string respawnPlayerPrefab = m_Gamemode.m_aSlotPrefabs.Get(index);
			vector position = SCR_PlayerController.GetLocalControlledEntity().GetOrigin();
			int groupID = SCR_AIGroup.Cast(RplComponent.Cast(Replication.FindItem(m_Gamemode.m_aActivePlayerGroupsIDs.Get(m_Gamemode.m_aGroupRplIDs.Find(m_Gamemode.m_aPlayerGroupIDs.Get(index))))).GetEntity()).GetGroupID();
			SCR_PlayerController.Cast(GetGame().GetPlayerController()).Respawn(playerID, respawnPlayerPrefab, position, groupID);
		}
		else
		{
			string respawnPlayerPrefab = "{577C074AAF8AE737}Prefabs/Characters/Factions/BLUFOR/Chosen Company/Character_Chosen_Rifleman.et";
			vector position = SCR_PlayerController.GetLocalControlledEntity().GetOrigin();
			int groupID = SCR_GroupsManagerComponent.GetInstance().GetPlayerGroup(SCR_PlayerController.GetLocalPlayerId()).GetGroupID();
			SCR_PlayerController.Cast(GetGame().GetPlayerController()).Respawn(playerID, respawnPlayerPrefab, position, groupID);
		}
	}
	
	void UpdateSlots()
	{
		m_wPlayerList.Clear();
		int alivePlayers = 0;
		RplId localGroupID = RplComponent.Cast(SCR_GroupsManagerComponent.GetInstance().GetPlayerGroup(SCR_PlayerController.GetLocalPlayerId()).FindComponent(RplComponent)).Id();
		for(int i = 0; i < m_Gamemode.m_aEntitySlots.Count(); i++)
		{
			if(m_Gamemode.m_aSlots.Get(i) <= 0)
				continue;
				
			if(m_bGroupToggle && m_Gamemode.m_aActivePlayerGroupsIDs.Get(m_Gamemode.m_aGroupRplIDs.Find(m_Gamemode.m_aPlayerGroupIDs.Get(i))) != localGroupID)
					continue;
			
			int index = m_wPlayerList.AddItemPlayer(null, m_Gamemode.m_aEntitySlots.Get(i), m_Gamemode.m_aSlots.Get(i));
			CHO_ListBoxElementComponent.Cast(m_wPlayerList.GetElementComponent(index)).GetRespawnButton().m_OnClicked.Insert(RespawnButtonDelay);
		}
		ref array<int> playerIDs = {};
		GetGame().GetPlayerManager().GetAllPlayers(playerIDs);
		foreach(int playerID: playerIDs)
		{
			if(SCR_FactionManager.SGetPlayerFaction(playerID).GetFactionKey() != "SPEC")
				alivePlayers++;
			
			if(!m_bGroupToggle && SCR_FactionManager.SGetPlayerFaction(playerID).GetFactionKey() == "SPEC" && m_Gamemode.m_aSlots.Find(playerID) == -1)
			{
				int index = m_wPlayerList.AddItemPlayer(null, RplId.Invalid(), playerID);
				CHO_ListBoxElementComponent.Cast(m_wPlayerList.GetElementComponent(index)).GetRespawnButton().m_OnClicked.Insert(RespawnButtonDelay);
			}
		}
		
		int totalPlayers = playerIDs.Count();
		m_wAliveText.SetText(alivePlayers.ToString() + "/" + totalPlayers.ToString() + " Alive");
		
		localSlotChanges = m_Gamemode.m_iSlotChanges;
	}
	
	void RespawnButtonDelay()
	{
		GetGame().GetCallqueue().CallLater(RespawnButton, 100, false);
	}
	
	void RespawnButton()
	{
		Respawn(CHO_ListBoxElementComponent.Cast(m_wPlayerList.GetElementComponent(m_wPlayerList.GetSelectedItem())).m_iSlotPlayerID);
	}
	
	override void OnMenuUpdate(float tDelta)
	{
		if(localSlotChanges != m_Gamemode.m_iSlotChanges)
			UpdateSlots();
	}
	
	override void OnMenuClose()
	{
		GetGame().GetInputManager().RemoveActionListener("MenuBack", EActionTrigger.DOWN, Action_Exit);
	}
	
	void Action_Exit()
	{
		Close();
	}
}