[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class GREG_SCR_NightVisionToolbarAction : SCR_BaseToggleToolbarAction
{
	private bool _isOn;
	int camId;
	
	override bool IsServer()
	{
		return false;
	}
	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return true;
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition,int flags, int param = -1)
	{	
		
		camId = GetGame().GetWorld().GetCurrentCameraId();
		_isOn = !_isOn;

		if (_isOn) 
		{
			SetEffects();
		} 
		else
		{
			ResetEffects();
		}
	}

	override void Untrack()
	{
		ResetEffects();
	}
	
	private void SetEffects()
	{
		camId = GetGame().GetWorld().GetCurrentCameraId();
		GetGame().GetWorld().SetCameraPostProcessEffect(camId, 16, PostProcessEffectType.HDR, "{7C2AA7F262C214A0}Assets/Shader/NVG/Color/Green/Editor_NightVisionGreen.emat"); // !!! MUST NOT BE MODIFIED HDR ALWAYS SET TO PRIORITY 16 !!!
	}
 
	private void ResetEffects()
	{
		camId = GetGame().GetWorld().GetCurrentCameraId();
		GetGame().GetWorld().SetCameraPostProcessEffect(camId, 16, PostProcessEffectType.HDR, "{9DEECCABE8357209}Common/Postprocess/HDR.emat"); // !!! MUST NOT BE MODIFIED !!!
		GetGame().GetWorld().SetCameraHDRBrightness(camId, -1); // To be sure we are resetting the value of the HDR Brightness.
	}
};
	
