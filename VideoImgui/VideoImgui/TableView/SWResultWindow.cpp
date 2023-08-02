#include "SWResultWindow.h"

std::vector<SWTableHeader> TableRowData::Headers = {
        {"NO",              ImGuiTableColumnFlags_WidthFixed,  TableColumnUIFlag_CheckBox},
        {"Check",           ImGuiTableColumnFlags_WidthFixed,   TableColumnUIFlag_Selected},
        {"Mark",            ImGuiTableColumnFlags_WidthFixed,   TableColumnUIFlag_Button},
        {"Related",         ImGuiTableColumnFlags_WidthFixed,   TableColumnUIFlag_EditBox},
        {"Project Name",    ImGuiTableColumnFlags_WidthFixed, TableColumnUIFlag_ComboBox},
        {"Formula",         ImGuiTableColumnFlags_WidthFixed,   TableColumnUIFlag_None},
        {"Total Score",     ImGuiTableColumnFlags_WidthFixed, TableColumnUIFlag_None},
};

ImVector<SWTableCell> TableRowData::SerializeToCell()
{
    if (!RowCells.empty())
    {
        return RowCells;
    }
    RowCells.resize(7, SWTableCell());

    RowCells[0] = SWTableCell(std::make_shared<SWCheckBoxCell>(NO, 1));
    RowCells[1] = SWTableCell(std::make_shared<SWSelectedCell>(std::to_string(Check).c_str()));
    RowCells[2] = SWTableCell(std::make_shared<SWBaseCell>(std::to_string(Mark).c_str()));
    RowCells[3] = SWTableCell(std::make_shared<SWBaseCell>(std::to_string(Related).c_str()));
    RowCells[4] = SWTableCell(std::make_shared<SWComboBoxCell>(ProjectName, 1));
    std::dynamic_pointer_cast<SWComboBoxCell>(RowCells[4].pCell)->Items = {
        { "Default",                           },
        { "ImGuiTableFlags_SizingFixedFit"     },
        { "ImGuiTableFlags_SizingFixedSame"    },
        { "ImGuiTableFlags_SizingStretchProp"  },
        { "ImGuiTableFlags_SizingStretchSame"  }
    };
    RowCells[5] = SWTableCell(std::make_shared<SWBaseCell>(Formula));
    RowCells[6] = SWTableCell(std::make_shared<SWBaseCell>(std::to_string(TotalScore).c_str()));

    return RowCells;
}


SWResultWindow& SWResultWindow::GetInstance()
{
    static SWResultWindow instance;
    // 初始化一次
    static bool isInit = false;
    if (!isInit)
    {
        instance.Init();
        isInit = true;
    }
   
    return instance;
}

void SWResultWindow::Show(bool* p_open)
{
    // Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
    // Most functions would normally just crash if the context is missing.
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

    // Demonstrate the various window flags. Typically you would just use the default!
    static bool no_close = false;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkSize.y), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("SW Computer", p_open, window_flags))
    {
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader("Horizontal scrolling"))
    {
        ImGui::SeparatorText("Child windows");
        CTableView::Show(TableData);
    }
    ImGui::End();
}

void SWResultWindow::ShowTable()
{
    CTableView::Show(TableData);
}

void SWResultWindow::Init()
{
    // 表属性
    TableData.ID = "result";
    TableData.Flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg
                    | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable
                    | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
                    ;// | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoHostExtendY;
    TableData.OuterSize = ImVec2(0.0f, 0.0f);
    // 表头
    TableData.Headers = TableRowData::Headers;
    // 加载表数据
    LoadTableData();
}

void SWResultWindow::LoadTableData()
{
    const int itemsCount = 10;
    TableData.Rows.resize(itemsCount, ImVector<SWTableCell>());

    int sum = 0;
    std::string formula = "0";
    for (int i = 1; i <= itemsCount; i++)
    {
        std::string id = std::to_string(i);
        formula = formula + "+" + id;
        sum += i;
        TableRowData item(i * 10, id.c_str(), 1, 0, 1,
            std::string("grep pip no." + id).c_str(), formula.c_str(), sum);

        // 添加
        TableData.Rows[i - 1] = item.SerializeToCell();
    }
}
