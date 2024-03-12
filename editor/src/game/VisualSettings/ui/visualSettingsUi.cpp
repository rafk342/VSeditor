#include "visualSettingsUi.h"

VisualSettingsUi::VisualSettingsUi(BaseUiWindow* base, const char* label) : App(base, label)
{

}

void VisualSettingsUi::window()
{
	static float col3[3];

	for (auto category : mHandler.mContainer.categoriesOrder)
	{
		if (ImGui::CollapsingHeader(category))
		{
			if (!mHandler.mContainer.paramsMap.contains(category))
				continue;

			auto& Params = mHandler.mContainer.paramsMap.at(category);

			for (size_t i = 0; i < Params.size(); i++)
			{
				if (!Params[i].found || !Params[i].gPtrItem)
					return;

				switch (Params[i].vType)
				{
				case Vs_VarType_e::V_NONE:
					break;

				case Vs_VarType_e::V_FLOAT:
					if (ImGui::DragFloat(Params[i].name, &Params[i].gPtrItem->value, 0.002f))
					{
						mHandler.updateData();
					}
					break;

				case Vs_VarType_e::V_COL3:

					for (u8 w = 0; w < 3; w++)
						col3[w] = Params[i + w].gPtrItem->value;

					if (ImGui::ColorEdit3(Params[i].name, col3))
					{
						for (u8 w = 0; w < 3; w++)
							Params[i + w].gPtrItem->value = col3[w];
						mHandler.updateData();
					}
					break;

				default:
					break;
				}
			}
		}
	}
}

void VisualSettingsUi::importData(std::string path)
{

}

void VisualSettingsUi::exportData(std::string path)
{

}

void visualSettings_fParser::importVsData()
{

}

void visualSettings_fParser::exportVsData()
{

}