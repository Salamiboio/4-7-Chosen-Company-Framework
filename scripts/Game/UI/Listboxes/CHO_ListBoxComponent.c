class CHO_ListboxComponent: SCR_ListBoxComponent
{
	CLB_Gamemode m_Gamemode;
	int AddItemPlayer(Managed data = null, RplId entityID = RplId.Invalid(), int playerID = 0)
	{	
		CHO_ListBoxElementComponent comp;
		
		int id = _AddItemPlayer(data, comp, entityID, playerID);
		
		return id;
	}
	
	protected int _AddItemPlayer(Managed data, out CHO_ListBoxElementComponent compOut, RplId entityID = RplId.Invalid(), int playerID = 0)
	{	
		// Create widget for this item
		// The layout can be provided either as argument or through attribute
		Widget newWidget = GetGame().GetWorkspace().CreateWidgets("{4B1BA5F8E3442E94}UI/layouts/Listbox/PlayerListboxElement.layout", m_wList);
		
		CHO_ListBoxElementComponent comp = CHO_ListBoxElementComponent.Cast(newWidget.FindHandler(CHO_ListBoxElementComponent));
		m_Gamemode = CLB_Gamemode.GetInstance();
		comp.m_iSlotPlayerID = playerID;
		Print(comp.m_iSlotPlayerID);
		
		comp.SetPlayerName(GetGame().GetPlayerManager().GetPlayerName(playerID));
		if(entityID != RplId.Invalid())
			comp.SetStatus(m_Gamemode.m_aEntityDeathStatus.Get(m_Gamemode.m_aEntitySlots.Find(entityID)));
		else
			comp.SetStatus(true);
		comp.SetToggleable(true);
		comp.SetData(data);
		
		// Pushback to internal arrays
		int id = m_aElementComponents.Insert(comp);
		
		// Setup event handlers
		comp.m_OnClicked.Insert(OnItemClick);
		
		// Set up explicit navigation rules for elements. Otherwise we can't navigate
		// Through separators when we are at the edge of scrolling if there is an element
		// directly above/below the list box which intercepts focus
		string widgetName = this.GetUniqueWidgetName();
		newWidget.SetName(widgetName);
		if (m_aElementComponents.Count() > 1)
		{
			Widget prevWidget = m_aElementComponents[m_aElementComponents.Count() - 2].GetRootWidget();
			prevWidget.SetNavigation(WidgetNavigationDirection.DOWN, WidgetNavigationRuleType.EXPLICIT, newWidget.GetName());
			newWidget.SetNavigation(WidgetNavigationDirection.UP, WidgetNavigationRuleType.EXPLICIT, prevWidget.GetName());
		}
		
		compOut = comp;
		
		return id;
	}
	
}