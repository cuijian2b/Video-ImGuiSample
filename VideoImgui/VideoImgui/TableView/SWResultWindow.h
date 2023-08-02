#pragma once
#include "CTableView.h"

namespace
{
    class TableRowData
    {
    public:
        TableRowData() = default;
        // ���ݳ�ʼ��
        TableRowData(int id, const char* no, int check, int mark, int related,
                        const char* projectName, const char* formula, int totalScore)
                        : ID(id), NO(no), Check(check), Mark(mark), Related(related),
                        ProjectName(projectName), Formula(formula), TotalScore(totalScore)
        {
        }
        virtual ~TableRowData() = default;

        // ���ݽṹ���л�ΪSWTableCell
        ImVector<SWTableCell> SerializeToCell();

        static std::vector<SWTableHeader> Headers;  // ��ͷ

    private:
        int                 ID;             // ѡ
        ImGuiTextBuffer     NO;             // ���
        int                 Check;          // ѡ
        int                 Mark;           // ���
        int                 Related;        // ����
        ImGuiTextBuffer     ProjectName;    // ��Ŀ����
        ImGuiTextBuffer     Formula;        // ����ʽ
        int                 TotalScore;     // �ܷ�

        ImVector<SWTableCell> RowCells;     // ��Ԫ����Ϣ
    };
}

class SWResultWindow
{
public:
    static SWResultWindow& GetInstance();

    // ��ʾ
    void Show(bool* p_open);
    void ShowTable();

private:
    // ��ʼ��
    void Init();
    // ���ر������
    void LoadTableData();

    SWResultWindow() = default;
    ~SWResultWindow() = default;

    SWTableWindow TableData;
};

