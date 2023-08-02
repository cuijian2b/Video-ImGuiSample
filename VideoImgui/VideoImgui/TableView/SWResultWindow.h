#pragma once
#include "CTableView.h"

namespace
{
    class TableRowData
    {
    public:
        TableRowData() = default;
        // 数据初始化
        TableRowData(int id, const char* no, int check, int mark, int related,
                        const char* projectName, const char* formula, int totalScore)
                        : ID(id), NO(no), Check(check), Mark(mark), Related(related),
                        ProjectName(projectName), Formula(formula), TotalScore(totalScore)
        {
        }
        virtual ~TableRowData() = default;

        // 数据结构序列化为SWTableCell
        ImVector<SWTableCell> SerializeToCell();

        static std::vector<SWTableHeader> Headers;  // 表头

    private:
        int                 ID;             // 选
        ImGuiTextBuffer     NO;             // 序号
        int                 Check;          // 选
        int                 Mark;           // 标记
        int                 Related;        // 关联
        ImGuiTextBuffer     ProjectName;    // 项目名称
        ImGuiTextBuffer     Formula;        // 计算式
        int                 TotalScore;     // 总分

        ImVector<SWTableCell> RowCells;     // 单元格信息
    };
}

class SWResultWindow
{
public:
    static SWResultWindow& GetInstance();

    // 显示
    void Show(bool* p_open);
    void ShowTable();

private:
    // 初始化
    void Init();
    // 加载表格数据
    void LoadTableData();

    SWResultWindow() = default;
    ~SWResultWindow() = default;

    SWTableWindow TableData;
};

