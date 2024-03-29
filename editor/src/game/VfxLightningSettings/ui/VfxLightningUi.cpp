#include "VfxLightningUi.h"


VfxLightningUi::VfxLightningUi(BaseUiWindow* base, const char* label) : App(base, label) {}


void VfxLightningUi::window()
{
	static bool mOverride_state = false;
	static int idx;
	static const char* typeNames[4] =
	{
		"None",
		"Directional Burst",
		"Cloud Burst",
		"Strike",
	};
	
	if (ImGui::TreeNode("Override##T_override"))
	{
		PushStyleCompact();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);

		if (ImGui::Checkbox("Enable override##T_override", &mOverride_state))
		{
			mVfxLightingHandler.setOverrideState(mOverride_state);
		}

		if (mOverride_state)
		{
			if (ImGui::Combo("Type Name##T_override", (int*)&idx, typeNames, 4))
			{
				mVfxLightingHandler.setCurrentUpdateType(0);
				mVfxLightingHandler.setOverrideType(idx);
			}
		}

		PopStyleCompact();
		ImGui::TreePop();
	}
	
	static float offset = 148.0f;
	if (ImGui::TreeNode("Requests"))
	{
		PushStyleCompact();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		ImGui::Text("Clear Update Type");
		ImGui::SameLine();
		ImGui::SetCursorPosX(offset);
		if (ImGui::Button("Request##Force Clear")) {
			mVfxLightingHandler.setCurrentUpdateType(0);
		}
		
		ImGui::Text("Directional Burst");
		ImGui::SameLine();
		ImGui::SetCursorPosX(offset);
		if (ImGui::Button("Request##Directional Burst")) {
			mVfxLightingHandler.DirBurstSeqRequest();
		}
		
		ImGui::Text("Cloud Burst");
		ImGui::SameLine();
		ImGui::SetCursorPosX(offset);
		if (ImGui::Button("Request##Cloud Lightning ")) {
			mVfxLightingHandler.CloudLightningSeqRequest();
		}
		
		ImGui::Text("Lightning Strike");
		ImGui::SameLine();
		ImGui::SetCursorPosX(offset);
		if (ImGui::Button("Request##Lightning")) {
			mVfxLightingHandler.LightningRequest();
		}
	
		PopStyleCompact();
		ImGui::TreePop();
	}
	ImGui::Separator();
	
	ImGui::Text("Current Update Type : %i", mVfxLightingHandler.getCurrentUpdateType());
	
	PushStyleCompact();
	
	if (!mOverride_state) {
		ImGui::DragInt("Lightning Occurrance Chance", &mVfxLightingHandler.mVfxLightningSettings->lightningOccurranceChance, 1, 0, 200);
	}
	ImGui::DragFloat("Lightning Shake Intensity", &mVfxLightingHandler.mVfxLightningSettings->lightningShakeIntensity, 0.0025, 0.0f, 1.0f);
	
	PopStyleCompact();
	
	LightningTimeCycleModsWidgets();
	DirectionalBurstSettingsWidgets();
	CloudBurstSettingsWidgets();
	StrikeSettingsWidgets();
}


void VfxLightningUi::LightningTimeCycleModsWidgets()
{
#if 0
	if (ImGui::CollapsingHeader("TimeCycle Modifiers"))
	{

	}
#endif
}


void VfxLightningUi::DirectionalBurstSettingsWidgets()
{
	static auto& dirBurstSettings = mVfxLightingHandler.mVfxLightningSettings->m_DirectionalBurstSettings;

	if (ImGui::CollapsingHeader("Directional Burst Settings"))
	{
		PushStyleCompact();
		if (ImGui::TreeNode("Main Settings ##__DirBurst"))
		{
			ImGui::DragFloat("Intensity Min ##__DirBurstSettings", &dirBurstSettings.BurstIntensityMin, 0.1f, 0.0f, 3000.0f);
			ImGui::DragFloat("Intensity Max ##__DirBurstSettings", &dirBurstSettings.BurstIntensityMax, 0.1f, 0.0f, 3000.0f);
			ImGui::DragFloat("Duration Min ##__DirBurstSettings", &dirBurstSettings.BurstDurationMin, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("Duration Max ##__DirBurstSettings", &dirBurstSettings.BurstDurationMax, 0.01f, 0.0f, 10.0f);
			ImGui::DragInt("Max Sequence Count ##__DirBurstSettings", &dirBurstSettings.BurstSeqCount, 0.5f, 1, 10);
			ImGui::DragFloat("Root Orbit X Variance ##__DirBurstSettings", &dirBurstSettings.RootOrbitXVariance, 0.1f, 0.0f, 180.0f);
			ImGui::DragFloat("Root Orbit Y Min ##__DirBurstSettings", &dirBurstSettings.RootOrbitYVarianceMin, 0.1f, -180.0f, 180.0f);
			ImGui::DragFloat("Root Orbit Y Max ##__DirBurstSettings", &dirBurstSettings.RootOrbitYVarianceMax, 0.1f, -180.0f, 180.0f);
			ImGui::DragFloat("Sequence Orbit X Variance ##__DirBurstSettings", &dirBurstSettings.BurstSeqOrbitXVariance, 0.1f, -180.0f, 180.0f);
			ImGui::DragFloat("Sequence Orbit Y Variance ##__DirBurstSettings", &dirBurstSettings.BurstSeqOrbitYVariance, 0.1f, -180.0f, 180.0f);
			
			ImGui::TreePop();
		}
		PopStyleCompact();
	}
}

void VfxLightningUi::CloudBurstCommonSettingsWidgets(CloudBurstCommonSettings& rCloudBurstCommonSettings, const char* treeLabel)
{
	static char buff[256];

	if (ImGui::TreeNode(treeLabel))
	{
		FORMAT_TO_BUFF(buff, "Burst Duration Min ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.BurstDurationMin, 0.1f, 0.0f, 10.f);

		FORMAT_TO_BUFF(buff, "Burst Duration Max ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.BurstDurationMax, 0.1f, 0.0f, 10.f);
		
		FORMAT_TO_BUFF(buff, "Burst Sequence Count ## {}", treeLabel);
		ImGui::DragInt(buff, &rCloudBurstCommonSettings.BurstSeqCount, 1, 1, 10);

		FORMAT_TO_BUFF(buff, "Root Orbit X Variance ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.RootOrbitXVariance, 0.1f, 0.0f, 180.0f);

		FORMAT_TO_BUFF(buff, "Root Orbit Y Min ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.RootOrbitYVarianceMin, 0.1f, 0.0f, 90.0f);

		FORMAT_TO_BUFF(buff, "Root Orbit Y Max ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.RootOrbitYVarianceMax, 0.1f, 0.0f, 90.0f);

		FORMAT_TO_BUFF(buff, "Local Orbit X Variance ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.LocalOrbitXVariance, 0.1f, -180.0f, 180.0f);

		FORMAT_TO_BUFF(buff, "Local Orbit Y Variance ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.LocalOrbitYVariance, 0.1f, -180.0f, 180.0f);

		FORMAT_TO_BUFF(buff, "Sequence Orbit X Variance ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.BurstSeqOrbitXVariance, 0.1f, -180.0f, 180.0f);

		FORMAT_TO_BUFF(buff, "Sequence Orbit Y Variance ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.BurstSeqOrbitYVariance, 0.1f, -180.0f, 180.0f);

		FORMAT_TO_BUFF(buff, "Delay Min ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.DelayMin, 0.1f , 0.0f, 90.0f);

		FORMAT_TO_BUFF(buff, "Delay Max ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.DelayMax, 0.1f, 0.0f, 90.0f);

		FORMAT_TO_BUFF(buff, "Size Min ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.SizeMin, 0.1f, 0.0f, 5000.0f);

		FORMAT_TO_BUFF(buff, "Size Max ## {}", treeLabel);
		ImGui::DragFloat(buff, &rCloudBurstCommonSettings.SizeMax, 0.1f, 0.0f, 5000.0f);

		ImGui::TreePop();
	}
}

void VfxLightningUi::CloudBurstSettingsWidgets()
{
	static auto& rCloudBurstSettings = mVfxLightingHandler.mVfxLightningSettings->m_CloudBurstSettings;
	static auto& rCloudBurstCommonSettings = mVfxLightingHandler.mVfxLightningSettings->m_CloudBurstSettings.m_CloudBurstCommonSettings;
	float* col;

	if (ImGui::CollapsingHeader("Cloud Burst Settings"))
	{
		PushStyleCompact();
		
		if (ImGui::TreeNode("Main Settings ##__ClBurst"))
		{
			ImGui::DragFloat("Intensity Min ##__CLBurstSettings", &rCloudBurstSettings.BurstIntensityMin, 0.1f, 0.0f, 3000.0f);
			ImGui::DragFloat("Intensity Max ##__CLBurstSettings", &rCloudBurstSettings.BurstIntensityMax, 0.1f, 0.0f, 3000.0f);
			ImGui::DragFloat("Light Source Exponent Fall Off ##__CLBurstSettings", &rCloudBurstSettings.LightSourceExponentFalloff, 0.1f, 0.0f, 128.0f);
			ImGui::DragFloat("Light Source Delta Position ##__CLBurstSettings", &rCloudBurstSettings.LightDeltaPos, 0.1f, -5000.f, 5000.f);
			ImGui::DragFloat("Light Source Distance ##__CLBurstSettings", &rCloudBurstSettings.LightDistance, 0.1f, 0.f, 5000.f);

			col = rCloudBurstSettings.LightColor.GetFloatRGBA();
			if (ImGui::ColorEdit4("Light Color", col)) {
				rCloudBurstSettings.LightColor.SetHexFromFloat(col);
			}
			ImGui::TreePop();
		}
		CloudBurstCommonSettingsWidgets(rCloudBurstCommonSettings, "Cloud Burst Common Settings");
		
		PopStyleCompact();
	}
}

void VfxLightningUi::StrikeSettingsWidgets()
{
	static auto& rStrikeSettings = mVfxLightingHandler.mVfxLightningSettings->m_StrikeSettings;
	static float* col;

	if (ImGui::CollapsingHeader("Strike Settings"))
	{
		PushStyleCompact();

		if (ImGui::TreeNode("Main Settings ## __CLStrikeSettings"))
		{
			ImGui::DragFloat("Intensity Min ##__CLStrikeSettings", &rStrikeSettings.IntensityMin, 0.1f, 0.0f, 50.0f);
			ImGui::DragFloat("Intensity Max ##__CLStrikeSettings", &rStrikeSettings.IntensityMax, 0.1f, 0.0f, 50.0f);
			ImGui::DragFloat("Intensity Min Clamp ##__CLStrikeSettings", &rStrikeSettings.IntensityMinClamp, 0.1f, 0.0f, 50.0f);

			ImGui::DragFloat("Width Min ##__CLStrikeSettings", &rStrikeSettings.WidthMin, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Width Max ##__CLStrikeSettings", &rStrikeSettings.WidthMax, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Width Min Clamp ##__CLStrikeSettings", &rStrikeSettings.WidthMinClamp, 0.1f, 0.0f, 100.0f);

			ImGui::DragFloat("Intensity Flicker Min ##__CLStrikeSettings", &rStrikeSettings.IntensityFlickerMin, 0.1f, 0.0f, 2.0f);
			ImGui::DragFloat("Intensity Flicker Max ##__CLStrikeSettings", &rStrikeSettings.IntensityFlickerMax, 0.1f, 0.0f, 2.0f);
			ImGui::DragFloat("Intensity Flicker Frequency ##__CLStrikeSettings", &rStrikeSettings.IntensityFlickerFrequency, 0.1f, 1.0f, 200.0f);

			ImGui::DragFloat("Intensity Level Decay Min ##__CLStrikeSettings", &rStrikeSettings.IntensityLevelDecayMin, 0.1f, 0.0f, 1.0f);
			ImGui::DragFloat("Intensity Level Decay Max ##__CLStrikeSettings", &rStrikeSettings.IntensityLevelDecayMax, 0.1f, 0.0f, 1.0f);
			ImGui::DragFloat("Width Level Decay Min ##__CLStrikeSettings", &rStrikeSettings.WidthLevelDecayMin, 0.1f, 0.0f, 1.0f);
			ImGui::DragFloat("Width Level Decay Max ##__CLStrikeSettings", &rStrikeSettings.WidthLevelDecayMax, 0.1f, 0.0f, 1.0f);

			ImGui::DragFloat("Noise Tex Scale ##__CLStrikeSettings", &rStrikeSettings.NoiseTexScale, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Noise Amplitude ##__CLStrikeSettings", &rStrikeSettings.NoiseAmplitude, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Noise Amp Min Dist Lerp ##__CLStrikeSettings", &rStrikeSettings.NoiseAmpMinDistLerp, 0.1f, 0.0f, 10000.0);
			ImGui::DragFloat("Noise Amp Max Dist Lerp ##__CLStrikeSettings", &rStrikeSettings.NoiseAmpMaxDistLerp, 0.1f, 0.0f, 10000.0);

			int subdiv_count = static_cast<int>(rStrikeSettings.SubdivisionCount);
			if (ImGui::DragInt("Subdivisions Count  ##__CLStrikeSettings", &subdiv_count, 1.0f, 1, 5)) {
				if (subdiv_count > 5) subdiv_count = 5;
				if (subdiv_count < 1) subdiv_count = 1;
				rStrikeSettings.SubdivisionCount = static_cast<u8>(subdiv_count);
			}

			ImGui::DragFloat("Orbit Direction X Variance  ##__CLStrikeSettings", &rStrikeSettings.OrbitDirXVariance, 0.1f, 0.0f, 180.0f);
			ImGui::DragFloat("Orbit Direction Y Min  ##__CLStrikeSettings", &rStrikeSettings.OrbitDirYVarianceMin, 0.1f, 0.0, 90.0f);
			ImGui::DragFloat("Orbit Direction Y Max  ##__CLStrikeSettings", &rStrikeSettings.OrbitDirYVarianceMax, 0.1f, 0.0, 90.0f);
			ImGui::DragFloat("Max Distance for Exposure Reset  ##__CLStrikeSettings", &rStrikeSettings.MaxDistanceForExposureReset, 0.1f, 0.0f, 10000.0f);
			ImGui::DragFloat("Angular Width Factor  ##__CLStrikeSettings", &rStrikeSettings.AngularWidthFactor, 0.1f, 0.0f, 1.0f);
			ImGui::DragFloat("Max Height For Strikes  ##__CLStrikeSettings", &rStrikeSettings.MaxHeightForStrike, 0.1f, 0.0f, 2000.0f);
			ImGui::DragFloat("Corona Intensity Multiplier  ##__CLStrikeSettings", &rStrikeSettings.CoronaIntensityMult, 0.1f, 0.0f, 10.0f);
			ImGui::DragFloat("Corona Size Factor  ##__CLStrikeSettings", &rStrikeSettings.BaseCoronaSize, 0.1f, 0.0, 100.0);

			col = rStrikeSettings.BaseColor.GetFloatRGBA();
			if (ImGui::ColorEdit4("Base Color  ##__CLStrikeSettings", col)) {
				rStrikeSettings.BaseColor.SetHexFromFloat(col);
			}

			col = rStrikeSettings.FogColor.GetFloatRGBA();
			if (ImGui::ColorEdit4("Fog Color  ##__CLStrikeSettings", col)) {
				rStrikeSettings.FogColor.SetHexFromFloat(col);
			}
			ImGui::DragFloat("Cloud Light Radius  ##__CLStrikeSettings", &rStrikeSettings.CloudLightRadius, 0.1f, 0.0, 5000.0f);
			ImGui::DragFloat("Cloud Light Delta Position  ##__CLStrikeSettings", &rStrikeSettings.CloudLightDeltaPos, 0.1f, -5000.0f, 5000.0f);
			ImGui::DragFloat("Cloud Light Intensity Multiplier  ##__CLStrikeSettings", &rStrikeSettings.CloudLightIntensityMult, 0.1f, 0.0f, 2000.0f);
			ImGui::DragFloat("Cloud Light Exponent Fall Off  ##__CLStrikeSettings", &rStrikeSettings.CloudLightFallOffExponent, 0.1f, 0.0f, 128.0f);
			
			ImGui::DragFloat("Max Distance for Burst  ##__CLStrikeSettings", &rStrikeSettings.MaxDistanceForBurst, 0.1f, 0.0f, 10000.0f);
			ImGui::DragFloat("Burst Threshold Intensity  ##__CLStrikeSettings", &rStrikeSettings.BurstThresholdIntensity, 0.1f, 0.0f, 1.0f);
			
			ImGui::DragFloat("Lightning Fade Width  ##__CLStrikeSettings", &rStrikeSettings.LightningFadeWidth, 0.1f, 1.0f, 100.0f);
			ImGui::DragFloat("Lightning Fade Width Falloff Exponent  ##__CLStrikeSettings", &rStrikeSettings.LightningFadeWidthFalloffExp, 0.1f, 0.0f, 128.0f);
		
			ImGui::TreePop();
		}
		
		CloudBurstCommonSettingsWidgets(rStrikeSettings.m_CloudBurstCommonSettings, "Cloud Burst Common Settings ## StrikeSettings ");

		if (ImGui::TreeNode("Strike Variations"))
		{
			for (u8 i = 0; i < 3; i++)
			{
				StrikeVariationsWidgets(i);
			}
			ImGui::TreePop();
		}

		PopStyleCompact();
	}
}

void VfxLightningUi::StrikeVariationsWidgets(u8 idx)
{
	static char buff[128];
	auto& var = mVfxLightingHandler.mVfxLightningSettings->m_StrikeSettings.m_Variations[idx];
	
	FORMAT_TO_BUFF(buff, "Variation : {} ##__CLStrikeSettings", idx);
	if (ImGui::TreeNode(buff))
	{
		FORMAT_TO_BUFF(buff, "Duration Min ##__CLStrikeSettings {}", idx);
		ImGui::DragFloat(buff, &var.DurationMin, 0.1f, 0.0f, 10.0f);

		FORMAT_TO_BUFF(buff, "Duration Max ##__CLStrikeSettings {}", idx);
		ImGui::DragFloat(buff, &var.DurationMax, 0.1f, 0.0f, 10.0f);

		FORMAT_TO_BUFF(buff, "Animation Time Min ##__CLStrikeSettings {}", idx);
		ImGui::DragFloat(buff, &var.AnimationTimeMin, 0.01f, 0.0f, 1.0f);

		FORMAT_TO_BUFF(buff, "Animation Time Max ##__CLStrikeSettings {}", idx);
		ImGui::DragFloat(buff, &var.AnimationTimeMax, 0.01f, 0.0f, 1.0f);

		FORMAT_TO_BUFF(buff, "End Point Offset X Variance ##__CLStrikeSettings {}", idx);
		ImGui::DragFloat(buff, &var.EndPointOffsetXVariance, 0.01f, -1.0f, 1.0f);

		FORMAT_TO_BUFF(buff, "Subdivision Pattern Mask ##__CLStrikeSettings {}", idx);
		int v = static_cast<int>(var.SubdivisionPatternMask);
		if (ImGui::DragInt(buff, &v, 0.5f, 0, 255)) {
			if (v > 255) v = 255;
			if (v < 0) v = 0;
			var.SubdivisionPatternMask = static_cast<u8>(v);
		}

		FORMAT_TO_BUFF(buff, "Zig Zag Split Point ##__CLStrikeSettings {}", idx);
		if (ImGui::TreeNode(buff))
		{
			FORMAT_TO_BUFF(buff, "Fraction Min ##__CLStrikeSettings {}", idx)
				ImGui::DragFloat(buff, &var.mZigZagSplitPoint.FractionMin, 0.01f, 0.0f, 1.0f);

			FORMAT_TO_BUFF(buff, "Fraction Max ##__CLStrikeSettings {}", idx);
			ImGui::DragFloat(buff, &var.mZigZagSplitPoint.FractionMax, 0.01f, 0.0f, 1.0f);

			FORMAT_TO_BUFF(buff, "Deviation Decay ##__CLStrikeSettings {}", idx);
			ImGui::DragFloat(buff, &var.mZigZagSplitPoint.DeviationDecay, 0.1f, 0.0f, 32.0f);

			FORMAT_TO_BUFF(buff, "Deviation Right Variance ##__CLStrikeSettings {}", idx);
			ImGui::DragFloat(buff, &var.mZigZagSplitPoint.DeviationRightVariance, 0.01f, 0.0f, 1.0f);

			ImGui::TreePop();
		}

		FORMAT_TO_BUFF(buff, "Fork Zig Zag Split Point ##__CLStrikeSettingsFork {}", idx);
		if (ImGui::TreeNode(buff))
		{
			FORMAT_TO_BUFF(buff, "Fraction Min ##__CLStrikeSettingsFork {}", idx);
			ImGui::DragFloat(buff, &var.mForkZigZagSplitPoint.FractionMin, 0.01f, 0.0f, 1.0f);

			FORMAT_TO_BUFF(buff, "Fraction Max ##__CLStrikeSettingsFork {}", idx);
			ImGui::DragFloat(buff, &var.mForkZigZagSplitPoint.FractionMax, 0.01f, 0.0f, 1.0f);

			FORMAT_TO_BUFF(buff, "Deviation Decay ##__CLStrikeSettingsFork {}", idx);
			ImGui::DragFloat(buff, &var.mForkZigZagSplitPoint.DeviationDecay, 0.1f, 0.0f, 32.0f);

			FORMAT_TO_BUFF(buff, "Deviation Right Variance ##__CLStrikeSettingsFork {}", idx);
			ImGui::DragFloat(buff, &var.mForkZigZagSplitPoint.DeviationRightVariance, 0.01f, 0.0f, 1.0f);

			ImGui::TreePop();
		}

		FORMAT_TO_BUFF(buff, "Fork Point ##__CLStrikeSettings ForkP {}", idx);
		if (ImGui::TreeNode(buff))
		{
			FORMAT_TO_BUFF(buff, "Deviation Right Variance ##__CLStrikeSettings ForkP {}", idx);
			ImGui::DragFloat(buff, &var.mForkPoint.DeviationRightVariance, 0.01f, 0.0f, 1.0f);

			FORMAT_TO_BUFF(buff, "Deviation Forward Min ##__CLStrikeSettings ForkP {}", idx);
			ImGui::DragFloat(buff, &var.mForkPoint.DeviationForwardMin, 0.01f, 0.0f, 1.0f);

			FORMAT_TO_BUFF(buff, "Deviation Forward Max ##__CLStrikeSettings ForkP {}", idx);
			ImGui::DragFloat(buff, &var.mForkPoint.DeviationForwardMax, 0.01f, 0.0f, 1.0f);

			FORMAT_TO_BUFF(buff, "Length Min ##__CLStrikeSettings ForkP {}", idx);
			ImGui::DragFloat(buff, &var.mForkPoint.LengthMin, 0.01f, 0.0f, 1.0f);

			FORMAT_TO_BUFF(buff, "Length Max ##__CLStrikeSettings ForkP {}", idx);
			ImGui::DragFloat(buff, &var.mForkPoint.LengthMax, 0.01f, 0.0f, 1.0f);

			FORMAT_TO_BUFF(buff, "Length Decay ##__CLStrikeSettings ForkP {}", idx);
			ImGui::DragFloat(buff, &var.mForkPoint.LengthDecay, 0.01f, 0.0f, 32.0f);

			ImGui::TreePop();
		}

		FORMAT_TO_BUFF(buff, "Lightning Key Frames ##__CLStrikeSettings {}", idx);
		if (ImGui::TreeNode(buff))
		{
			FORMAT_TO_BUFF(buff, "Lightning Main Intensity ##__CLStrikeSettings {}", idx);
			if (ImGui::TreeNode(buff)) 
			{
				auto& data = var.mKeyFrameData.LightningMainIntensity.data;

				FORMAT_TO_BUFF(buff, "## Lightning Main__CLStrikeSettings table {}", idx);
				if (ImGui::BeginTable(buff, 3, ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 45);
					ImGui::TableSetupColumn("Min");
					ImGui::TableSetupColumn("Max");
					ImGui::TableHeadersRow();

					for (u8 i = 0; i < data.GetSize(); i++)
					{
						ImGui::TableNextRow();

						for (u8 j = 0; j < 3; j++)
						{
							ImGui::TableNextColumn();

							switch (j)
							{
							case 0:
								FORMAT_TO_BUFF(buff, " {:.3f} ", data[i].vTime[0], idx);
								ImGui::Text(buff);
								break;
							case 1:
								ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
								FORMAT_TO_BUFF(buff, "##MainIntensity{}_{}_{}__keyframe_Min", i, idx,j);
								ImGui::DragFloat(buff, &data[i].vValue[0], 0.015, 0.0f, 1.0f);
								break;
							case 2:
								ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
								FORMAT_TO_BUFF(buff, "##MainIntensity{}_{}_{}__keyframe_Max", i, idx,j);
								ImGui::DragFloat(buff, &data[i].vValue[1], 0.015, 0.0f, 1.0f);
								break;
							default:
								break;
							}
						}
					}
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}


			FORMAT_TO_BUFF(buff, "Lightning Branch Intensity ##__CLStrikeSettings {}", idx);
			if (ImGui::TreeNode(buff))
			{
				auto& data = var.mKeyFrameData.LightningBranchIntensity.data;

				FORMAT_TO_BUFF(buff, "## Lightning Branch__CLStrikeSettings table {}", idx);
				if (ImGui::BeginTable(buff, 3, ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 45);
					ImGui::TableSetupColumn("Min");
					ImGui::TableSetupColumn("Max");
					ImGui::TableHeadersRow();

					for (u8 i = 0; i < data.GetSize(); i++)
					{
						ImGui::TableNextRow();

						for (u8 j = 0; j < 3; j++)
						{
							ImGui::TableNextColumn();

							switch (j)
							{
							case 0:
								FORMAT_TO_BUFF(buff, " {:.3f} ", data[i].vTime[0], idx);
								ImGui::Text(buff);
								break;
							case 1:
								ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
								FORMAT_TO_BUFF(buff, "##BranchIntensity{}_{}_{}__keyframe_Min", i, idx, j);
								ImGui::DragFloat(buff, &data[i].vValue[0], 0.015, 0.0f, 1.0f);
								break;
							case 2:
								ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
								FORMAT_TO_BUFF(buff, "##BranchIntensity{}_{}_{}__keyframe_Max", i, idx, j);
								ImGui::DragFloat(buff, &data[i].vValue[1], 0.015, 0.0f, 1.0f);
								break;
							default:
								break;
							}
						}
					}
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}

void VfxLightningUi::importData(std::string path)
{
	mXmlParser.mImportLightningData(path, mVfxLightingHandler.mVfxLightningSettings);
}

void VfxLightningUi::exportData(std::string path)
{
	mXmlParser.mExportLightningData(path, mVfxLightingHandler.mVfxLightningSettings);
}
