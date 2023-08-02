#include "CTableView.h"


bool CTableView::Show(SWTableWindow& info)
{
    if (ImGui::BeginTable(info.ID, info.Headers.size(), info.Flags, info.OuterSize, info.InnerWidth))
    {
        ImGuiContext* Ctx = ImGui::GetCurrentContext();
        ImGuiContext& g = *Ctx;
        //pGuiTable = g.CurrentTable;

        // 添加表头
        AddTableInfo(info);

        ImGui::EndTable();
        return true;
    }

    return false;
}

void CTableView::AddTableInfo(SWTableWindow& info)
{
    std::vector<TableColumnUIFlag> columnUIType;   // UI类别信息
    for (auto& item : info.Headers)
    {
        ImGui::TableSetupColumn(item.Label, item.Flags, item.InitWidthOrWeight, item.UserId);
        columnUIType.push_back(item.UiType);
    }

    // Show headers
    ImGui::TableSetupScrollFreeze(1, 1);
    ImGui::TableHeadersRow();

    UpdateRowMsg(info, columnUIType);
}


void CTableView::UpdateRowMsg(SWTableWindow& info, std::vector<TableColumnUIFlag>& columnUIType)
{
    int cellIndex = 0;  // 单元格索引
    float row_min_height = 0.0;
    ImGuiListClipper clipper;
    clipper.Begin(info.Rows.Size);
    while (clipper.Step())
    {
        for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
        {
            ImGui::TableNextRow(ImGuiTableRowFlags_None);
            static int flags = 0;
            ImVector<SWTableCell>& item = info.Rows[row];
            for (int l = 0; l < item.Size; l++)
            {
                item[l].ID = cellIndex++;
                ImGui::PushID(item[l].ID);
                if (ImGui::TableSetColumnIndex(l))
                    switch (columnUIType[l])
                    {
                    case TableColumnUIFlag_Selected:
                        AddSelectedCell(info.Highlight.CellSelected, item[l]);
                        break;
                    case TableColumnUIFlag_Button:
                        ImGui::Button(item[l].pCell->Msg.c_str(), ImVec2(-FLT_MIN, 0.0f));
                        break;
                    case TableColumnUIFlag_EditBox:
                        AddEditBoxCell(item[l]);
                        break;
                    case TableColumnUIFlag_ComboBox:
                        AddComboBoxCell(item[l]);
                        break;
                    case TableColumnUIFlag_CheckBox:
                        {
                            std::shared_ptr<SWCheckBoxCell> cellInfo
                                    = std::dynamic_pointer_cast<SWCheckBoxCell>(item[l].pCell);
                            ImGui::CheckboxFlags(cellInfo->Msg.c_str(), &cellInfo->Flags, cellInfo->Value);
                        }
                        break;
                    default:
                        ImGui::Text(item[l].pCell->Msg.c_str());
                        break;
                    }
                ImGui::PopID();
            }
        }
    }
}

void CTableView::AddSelectedCell(ImVector<int>& cellSelected, SWTableCell& cell)
{
    const bool itemSelected = cellSelected.contains(cell.ID);
    if (ImGui::Selectable(cell.pCell->Msg.c_str(), itemSelected))
    {        
        cellSelected.clear();
        cellSelected.push_back(cell.ID);
    }
}

void CTableView::AddEditBoxCell(SWTableCell& cell)
{
    struct Funcs
    {
        static int EditResizeCallback(ImGuiInputTextCallbackData* data)
        {
            if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
            {
                ImVector<char>* editText = (ImVector<char>*)data->UserData;
                IM_ASSERT(editText->begin() == data->Buf);
                editText->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
                data->Buf = editText->begin();
            }
            return 0;
        }

        // Note: Because ImGui:: is a namespace you would typically add your own function into the namespace.
        // For example, you code may declare a function 'ImGui::InputText(const char* label, MyString* my_str)'
        static bool EditInputText(const char* label, ImVector<char>* editText, ImGuiInputTextFlags flags)
        {
            return ImGui::InputText(label, editText->begin(), (size_t)editText->size(), flags, Funcs::EditResizeCallback, (void*)editText);
        }
    };

    ImGui::SetNextItemWidth(-FLT_MIN);
    Funcs::EditInputText("##cell", &cell.pCell->Msg.Buf, ImGuiInputTextFlags_CallbackResize);
}

void CTableView::AddComboBoxCell(SWTableCell& cell)
{
    std::shared_ptr<SWComboBoxCell> cellInfo = std::dynamic_pointer_cast<SWComboBoxCell>(cell.pCell);
    if (ImGui::BeginCombo("", cellInfo->Items[cellInfo->Index].Name.c_str()))
    {
        for (int i = 0; i < cellInfo->Items.size(); i++)
        {
            if (ImGui::Selectable(cellInfo->Items[i].Name.c_str(), cellInfo->Index == i))
            {
                cellInfo->Index = i;
            }
        }
        ImGui::EndCombo();
    }
}
