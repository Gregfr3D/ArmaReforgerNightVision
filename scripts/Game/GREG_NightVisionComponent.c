/*
	Author: gregfr_3d
	Name: 
		GREG_NightVisionComponent.c
	Description:
		Create a new component called "GREG_NightVisionComponent". Put this component on your helmet and setup the color shader path and the grain shader path.
		You can create and use your custom shader without changing the script. The Color Shader include the Vignette or the Scope.
		For now you CAN'T rotate the Night Vision Goggles cause it's not supported yet.
		The key by default are :
					SHIFT + N = Turn On/Off Night Vision.
					SHIFT + "+" = Increase the luminosity of the Night Vision.
					SHIFT + Q (or A) = Decrease the luminosity of the Night Vision.
	If you have any troubles with this mod or script please contact me on discord : Greg🔥#0431
*/

[ComponentEditorProps(category: "GREG/Components", description: "Enables Night Vision", color: "0 0 255 255")]
class GREG_NightVisionComponentClass: ScriptComponentClass {};
 
class GREG_NightVisionComponent: ScriptComponent
{
	[Attribute("", UIWidgets.ResourceNamePicker, "Color Shader Path", "emat")]
	private ResourceName m_colorshaderpath;
 
	[Attribute("", UIWidgets.ResourceNamePicker, "Grain Shader Path", "emat")]
	private ResourceName m_grainshaderpath;
 
	[Attribute("false", UIWidgets.CheckBox, "Luminosity Can Be Changed ?")]
    protected bool m_CanChangeLuminosity;
 
	private bool _isOn;
	protected SoundComponent _soundComponent;
	private SCR_CharacterInventoryStorageComponent _inventory;
	private int camId;
	
	/* 
		Default value of the luminosity. 
		0 --> 3 Less luminosity, 
		3 --> 6 More luminosity
	*/
	
	private float LuminosityValue = 3;
 
	override void EOnInit(IEntity owner)
	{
		_soundComponent = SoundComponent.Cast(owner.FindComponent(SoundComponent));
		GetGame().GetInputManager().AddActionListener("NightVision", EActionTrigger.DOWN, KeybindPressed);
		GetGame().GetInputManager().AddActionListener("NightVisionIncreaseLuminosity", EActionTrigger.DOWN, IncreaseLuminosity);
		GetGame().GetInputManager().AddActionListener("NightVisionDecreaseLuminosity", EActionTrigger.DOWN, DecreaseLuminosity);
		BaseWorld world = owner.GetWorld();
		camId = world.GetCurrentCameraId();
        owner.SetFlags(EntityFlags.ACTIVE, true);
		SetEventMask(owner, EntityEvent.FRAME);
	}
 
	// We need the OnPostInit and EOnInit for the EOnFrame to be called.
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
	}
 
	protected IEntity GetPlayerOwningNVG()
	{
	  return GetOwner().GetParent();
	}
 
	private void KeybindPressed()
	{
		IEntity localPlayer = GetGame().GetPlayerController().GetControlledEntity();
 
		if(localPlayer == GetPlayerOwningNVG())
		{		
			_isOn = !_isOn;
			
			if(_isOn) 
			{
				SetEffects(m_colorshaderpath, m_grainshaderpath);
			}
			else 
			{
				ResetEffects();
			}
		}
	}
 
	private void SetEffects(string mat16, string mat17)
	{
		if(_soundComponent) _soundComponent.SoundEvent("Sound_turn_on_nvg");
		GetGame().GetWorld().SetCameraPostProcessEffect(camId, 16, PostProcessEffectType.HDR, mat16); // !!! MUST NOT BE MODIFIED HDR ALWAYS SET TO PRIORITY 16 !!!
		GetGame().GetWorld().SetCameraPostProcessEffect(camId, 18, PostProcessEffectType.FilmGrain, mat17);
	}
 
	void ResetEffects()
	{
		GetGame().GetWorld().SetCameraPostProcessEffect(camId, 16, PostProcessEffectType.HDR, "{9DEECCABE8357209}Common/Postprocess/HDR.emat"); // !!! MUST NOT BE MODIFIED !!!
		GetGame().GetWorld().SetCameraPostProcessEffect(camId, 18, PostProcessEffectType.None, "");
		GetGame().GetWorld().SetCameraPostProcessEffect(camId, 12, PostProcessEffectType.None, "");
 
		LuminosityValue = 3;
 
		GetGame().GetWorld().SetCameraHDRBrightness(camId, -1); // To be sure we are resetting the value of the HDR Brightness.
		
		_isOn = false;
	}
 
	private void IncreaseLuminosity()
	{
		if(_isOn) 
		{
			if ((LuminosityValue <= 5) && m_CanChangeLuminosity)
			{
				if(_soundComponent) _soundComponent.SoundEvent("Sound_button_pressed");
				LuminosityValue = LuminosityValue + 1;
				float DefaultHDRBrightness = GetGame().GetWorld().GetCameraHDRBrightness(camId);
				float HDRBrightness = DefaultHDRBrightness + 0.7;
				GetGame().GetWorld().SetCameraHDRBrightness(camId, HDRBrightness);
			}
		}
	}
 
	private void DecreaseLuminosity()
	{
		if(_isOn) 
		{
			if ((LuminosityValue >= 1) && m_CanChangeLuminosity)
			{
				if(_soundComponent) _soundComponent.SoundEvent("Sound_button_pressed");
				LuminosityValue = LuminosityValue - 1;
				float DefaultHDRBrightness = GetGame().GetWorld().GetCameraHDRBrightness(camId);
				float HDRBrightness = DefaultHDRBrightness - 0.7;
				GetGame().GetWorld().SetCameraHDRBrightness(camId, HDRBrightness);
			}
		}
	}		
}