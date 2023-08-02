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
    TableColumnUIFlag_None          = 0,                    // ��ͨ���� -> SWBaseCell
    TableColumnUIFlag_Selected      = 1 << 0,               // ������� -> SWSelectedCell
    TableColumnUIFlag_Button        = 1 << 1,               // ��ť -> SWBaseCell
    TableColumnUIFlag_EditBox       = 1 << 2,               // �༭�� -> SWBaseCell
    TableColumnUIFlag_ComboBox      = 1 << 3,               // ѡ����Ͽ� -> SWComboBoxCell
    TableColumnUIFlag_CheckBox      = 1 << 4,               // ѡ��� -> SWCheckBoxCell
};

// ��ͷ��Ϣ
struct SWTableHeader
{
    const char* Label;                  // ����
    ImGuiTableColumnFlags Flags;        // ����
    TableColumnUIFlag UiType;           // UI���
    ImGuiID UserId;                     // ID
    float InitWidthOrWeight;            // ��ʼ����ȣ�0.0

    SWTableHeader(const char* label, ImGuiTableColumnFlags flags = 0, TableColumnUIFlag ui_type = 0,
                ImGuiID user_id = 0, float init_width_or_weight = 0.0f)
                : Label(label), Flags(flags), UiType(ui_type),
                  UserId(user_id), InitWidthOrWeight(init_width_or_weight)
    {}
};

// ��ͨ��Ԫ����Ϣ
struct SWBaseCell
{    
    CellMsg Msg;                // ��Ϣ

    //SWBaseCell() = default;    

    SWBaseCell(CellMsg& msg) : Msg(msg)
    {
    }

    SWBaseCell(const char* msg) : Msg(msg)
    {
    }

    virtual ~SWBaseCell() = default;
};

// ��ѡ��Ԫ����Ϣ
struct SWSelectedCell : public SWBaseCell
{
    bool Selected;                 // ui��ʾ

    SWSelectedCell() = default;

    SWSelectedCell(CellMsg& msg) : SWBaseCell(msg), Selected(false)
    {
    }

    SWSelectedCell(const char* msg) : SWBaseCell(msg), Selected(false)
    {
    }
};

// ��ѡ��Ԫ����Ϣ
struct SWCheckBoxCell : public SWBaseCell
{
    int Flags;                  // ѡ�񷵻�ֵ
    int Value;                  // ��ѡʱֵ

    SWCheckBoxCell() = default;

    SWCheckBoxCell(CellMsg& msg, int value) : SWBaseCell(msg), Flags(0), Value(value)
    {
    }

    SWCheckBoxCell(const char* msg, int value) : SWBaseCell(msg), Flags(0), Value(value)
    {
    }
};

// ��ѡ��Ԫ����Ϣ
struct SWComboBoxCell : public SWBaseCell
{
    struct ComboBoxItem
    {
        CellMsg Name;

        ComboBoxItem() = default;
        ComboBoxItem(const char* name) : Name(name) {}
    };

    int Index;                          // ѡ������
    std::vector<ComboBoxItem> Items;    // ѡ������

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
    std::shared_ptr<SWBaseCell> pCell;      // ��Ԫ����Ϣ

    SWTableCell() : ID(0) {}
    SWTableCell(std::shared_ptr<SWBaseCell> cell) : ID(0)
    {
        pCell = cell;
    }

    virtual ~SWTableCell() = default;
};

// ��������Ϣ
struct SWTableHighlight
{
    ImVector<int> CellSelected;     // ��Ԫ��ѡ�и���
    ImVector<int> RowSelected;      // ��ѡ�и���
    ImVector<int> ColumnSelected;   // ��ѡ�и���
};

// ����Ϣ
struct SWTableWindow
{
    const char* ID = nullptr;                       // ������
    ImGuiTableFlags Flags = 0;                      // ������
    ImVec2 OuterSize = ImVec2(0.0f, 0.0f);          // ��С
    float InnerWidth = 0.0f;                        // �ڲ����

    SWTableWindow(const char* id = nullptr, ImGuiTableFlags flags = 0,
        ImVec2 outerSize = ImVec2(0.0f, 0.0f), float innerWidth = 0.0f)
        : ID(id), Flags(flags), OuterSize(outerSize), InnerWidth(innerWidth) {}

    std::vector<SWTableHeader> Headers;             // ��ͷ
    ImVector<ImVector<SWTableCell>> Rows;           // ������
    SWTableHighlight Highlight;                     // �������Ϣ
};

class CTableView
{
public:
    CTableView() = default;

    // ��ʾ
    static bool Show(SWTableWindow& info);

private:
    // ��ӱ�
    static void AddTableInfo(SWTableWindow& info);
    // ��ӱ����Ϣ
    static void UpdateRowMsg(SWTableWindow& info, std::vector<TableColumnUIFlag>& columnUIType);

    // ��ӿ�ѡ�������Ԫ��
    static void AddSelectedCell(ImVector<int>& cellSelected, SWTableCell& cell);
    // ��ӱ༭��Ԫ��
    static void AddEditBoxCell(SWTableCell& cell);
    // ������Ͽ��Ԫ��
    static void AddComboBoxCell(SWTableCell& cell);
};


