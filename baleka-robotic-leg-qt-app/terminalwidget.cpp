/*
 * Copyright (C) 2012 Jorge Aparicio <jorge.aparicio.r@gmail.com>
 *
 * This file is part of qSerialTerm.
 *
 * qSerialTerm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * qSerialTerm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with qSerialTerm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "terminalwidget.h"
#include "ui_terminalwidget.h"

TerminalWidget::TerminalWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TerminalWidget)
{
  ui->setupUi(this);
}

TerminalWidget::~TerminalWidget()
{
  delete ui;
}

void TerminalWidget::display(QByteArray const &text)
{
  if (text.length() != 0) {
    ui->terminalTextEdit->moveCursor(QTextCursor::End,
                                     QTextCursor::MoveAnchor);

    ui->terminalTextEdit->insertPlainText(text);
  }
}

void TerminalWidget::on_cleanPushButton_clicked()
{
  ui->terminalTextEdit->clear();
}