//===========================================================================
/*!
 *  \file ControlWidget.cpp
 *
 *  \author  T. Glasmachers
 *  \date    2008
 *
 *  \par Copyright (c) 2008:
 *      Institut f&uuml;r Neuroinformatik<BR>
 *      Ruhr-Universit&auml;t Bochum<BR>
 *      D-44780 Bochum, Germany<BR>
 *      Phone: +49-234-32-27974<BR>
 *      Fax:   +49-234-32-14209<BR>
 *      eMail: Shark-admin@neuroinformatik.ruhr-uni-bochum.de<BR>
 *      www:   http://www.neuroinformatik.ruhr-uni-bochum.de<BR>
 *
 *
 *  <BR><HR>
 *  This file is part of Shark. This library is free software;
 *  you can redistribute it and/or modify it under the terms of the
 *  GNU General Public License as published by the Free Software
 *  Foundation; either version 2, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
//===========================================================================


#include "ControlWidget.h"


ManualControlWidget::ManualControlWidget(QString experiment, QWidget* parent)
: QWidget(parent)
, label("", this)
, next("Next", this)
, stop("Cancel", this)
{
	wasNext = false;
	wasStop = false;

	setWindowTitle("Manual Control");
	setFixedSize(300, 100);

	label.setGeometry(0, 10, 280, 25);
	next.setGeometry(50, 50, 80, 30);
	stop.setGeometry(170, 50, 80, 30);
	label.setAlignment(Qt::AlignHCenter);
	label.setText(experiment);

	connect(&next, SIGNAL(pressed()), this, SLOT(onNext()));
	connect(&stop, SIGNAL(pressed()), this, SLOT(onStop()));
}

ManualControlWidget::~ManualControlWidget()
{
}


bool ManualControlWidget::isNext()
{
	bool ret = wasNext;
	wasNext = false;
	return ret;
}

bool ManualControlWidget::isStop()
{
	return wasStop;
}

void ManualControlWidget::onNext()
{
	if (wasNext) return;
	wasNext = true;
}

void ManualControlWidget::onStop()
{
	wasStop = true;
}

void ManualControlWidget::onDestroy()
{
	close();
}

void ManualControlWidget::closeEvent(QCloseEvent* event)
{
	delete this;
}