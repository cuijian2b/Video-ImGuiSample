#pragma once
#include <string>
#include <vector>
#include <memory>
#include "imgui.h"
#include "imgui_internal.h"

typedef ImGuiTextBuffer CellMsg;
typedef int TableColumnUIFlag; // -> enum TableColumnUIFlag_

enum TableColumnUIFlag_
{
    TableColumnUIFlag_None          = 0,                    // 普通文字 -> SWBaseCell
    TableColumnUIFlag_Selected      = 1 << 0,               // 点击高亮 -> SWSelectedCell
    TableColumnUIFlag_Button        = 1 << 1,               // 按钮 -> SWBaseCell
    TableColumnUIFlag_EditBox       = 1 << 2,               // 编辑框 -> SWBaseCell
    TableColumnUIFlag_ComboBox      = 1 << 3,               // 选择组合框 -> SWComboBoxCell
    TableColumnUIFlag_CheckBox      = 1 << 4,               // 选择框 -> SWCheckBoxCell
};

// 表头信息
struct SWTableHeader
{
    const char* Label;                  // 标题
    ImGuiTableColumnFlags Flags;        // 属性
    TableColumnUIFlag UiType;           // UI类别
    ImGuiID UserId;                     // ID
    float InitWidthOrWeight;            // 初始化宽度，0.0

    SWTableHeader(const char* label, ImGuiTableColumnFlags flags = 0, TableColumnUIFlag ui_type = 0,
                ImGuiID user_id = 0, float init_width_or_weight = 0.0f)
                : Label(label), Flags(flags), UiType(ui_type),
                  UserId(user_id), InitWidthOrWeight(init_width_or_weight)
    {}
};

// 普通单元格信息
struct SWBaseCell
{    
    CellMsg Msg;                // 信息

    //SWBaseCell() = default;    

    SWBaseCell(CellMsg& msg) : Msg(msg)
    {
    }

    SWBaseCell(const char* msg) : Msg(msg)
    {
    }

    virtual ~SWBaseCell() = default;
};

// 可选择单元格信息
struct SWSelectedCell : public SWBaseCell
{
    bool Selected;                 // ui标示

    SWSelectedCell() = default;

    SWSelectedCell(CellMsg& msg) : SWBaseCell(msg), Selected(false)
    {
    }

    SWSelectedCell(const char* msg) : SWBaseCell(msg), Selected(false)
    {
    }
};

// 可选择单元格信息
struct SWCheckBoxCell : public SWBaseCell
{
    int Flags;                  // 选择返回值
    int Value;                  // 勾选时值

    SWCheckBoxCell() = default;

    SWCheckBoxCell(CellMsg& msg, int value) : SWBaseCell(msg), Flags(0), Value(value)
    {
    }

    SWCheckBoxCell(const char* msg, int value) : SWBaseCell(msg), Flags(0), Value(value)
    {
    }
};

// 可选择单元格信息
struct SWComboBoxCell : public SWBaseCell
{
    struct ComboBoxItem
    {
        CellMsg Name;

        ComboBoxItem() = default;
        ComboBoxItem(const char* name) : Name(name) {}
    };

    int Index;                          // 选项索引
    std::vector<ComboBoxItem> Items;    // 选项详情

    SWComboBoxCell() = default;

    SWComboBoxCell(CellMsg& msg, int index = 0) : SWBaseCell(msg), Index(index)
    {
    }

    SWComboBoxCell(const char* msg, int index = 0) : SWBaseCell(msg), Index(index)
    {
    }
};

struct SWTableCell
{
    int ID;                                 // ID
    std::shared_ptr<SWBaseCell> pCell;      // 单元格信息

    SWTableCell() : ID(0) {}
    SWTableCell(std::shared_ptr<SWBaseCell> cell) : ID(0)
    {
        pCell = cell;
    }

    virtual ~SWTableCell() = default;
};

// 表格高亮信息
struct SWTableHighlight
{
    ImVector<int> CellSelected;     // 单元格选中高亮
    ImVector<int> RowSelected;      // 行选中高亮
    ImVector<int> ColumnSelected;   // 列选中高亮
};

// 表信息
struct SWTableWindow
{
    const char* ID = nullptr;                       // 表名称
    ImGuiTableFlags Flags = 0;                      // 表属性
    ImVec2 OuterSize = ImVec2(0.0f, 0.0f);          // 大小
    float InnerWidth = 0.0f;                        // 内部宽度

    SWTableWindow(const char* id = nullptr, ImGuiTableFlags flags = 0,
        ImVec2 outerSize = ImVec2(0.0f, 0.0f), float innerWidth = 0.0f)
        : ID(id), Flags(flags), OuterSize(outerSize), InnerWidth(innerWidth) {}

    std::vector<SWTableHeader> Headers;             // 表头
    ImVector<ImVector<SWTableCell>> Rows;           // 表数据
    SWTableHighlight Highlight;                     // 表高亮信息
};

class CTableView
{
public:
    CTableView() = default;

    // 显示
    static bool Show(SWTableWindow& info);

private:
    // 添加表
    static void AddTableInfo(SWTableWindow& info);
    // 添加表格信息
    static void UpdateRowMsg(SWTableWindow& info, std::vector<TableColumnUIFlag>& columnUIType);

    // 添加可选择高亮单元格
    static void AddSelectedCell(ImVector<int>& cellSelected, SWTableCell& cell);
    // 添加编辑框单元格
    static void AddEditBoxCell(SWTableCell& cell);
    // 下拉组合框框单元格
    static void AddComboBoxCell(SWTableCell& cell);
};


