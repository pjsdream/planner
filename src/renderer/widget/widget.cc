#include "renderer/widget/widget.h"

namespace renderer
{
Widget::Widget()
{
}

void Widget::AddWidget(int row, int col, const std::shared_ptr<Widget>& widget)
{
  bool layout_changed = false;
  if (row >= num_rows_)
  {
    num_rows_ = row + 1;
    layout_changed = true;
  }
  if (col >= num_cols_)
  {
    num_cols_ = col + 1;
    layout_changed = true;
  }

  if (layout_changed)
  {
    widgets_.resize(num_rows_);
    for (int i = 0; i < num_rows_; i++)
      widgets_[i].resize(num_cols_);
  }

  widgets_[row][col] = widget;

  if (layout_changed)
  {
    int width = width / num_cols_;
    int height = height / num_rows_;

    for (int i = 0; i < num_rows_; i++)
    {
      for (int j = 0; j < num_cols_; j++)
      {
        widgets_[i][j]->SetWidth(width);
        widgets_[i][j]->SetHeight(height);
      }
    }
  }
}

void Widget::Mouse(int button, int state, double x, double y)
{
}

void Widget::Motion(int button, int state, double x, double y)
{
}

void Widget::Idle()
{
}
}
