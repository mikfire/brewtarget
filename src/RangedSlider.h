/*
 * RangedSlider.h is part of Brewtarget, and is Copyright the following
 * authors 2009-2014
 * - Mik Firestone <mikfire@gmail.com>
 * - Philip G. Lee <rocketman768@gmail.com>
 *
 * Brewtarget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Brewtarget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RANGEDSLIDER_H
#define RANGEDSLIDER_H

#include <QWidget>
#include <QSize>
#include <QString>
#include <QBrush>
#include <QPen>
class QPaintEvent;
class QMouseEvent;

/*!
 * \brief Widget to display a number with an optional range on a type of read-only slider.
 * \author Philip G. Lee
 */
class RangedSlider : public QWidget
{
   Q_OBJECT
   
public:
   RangedSlider(QWidget* parent=0);
   
   Q_PROPERTY( double value READ value WRITE setValue )
   
   double value() const { return _val; }
   
   //! \brief Set the background brush for the widget.
   void setBackgroundBrush( QBrush const& brush );
   //! \brief Set the brush for the preffered range.
   void setPreferredRangeBrush( QBrush const& brush );
   //! \brief Set the pen for the preferred range
   void setPreferredRangePen( QPen const& pen );
   //! \brief Set the brush for the marker.
   void setMarkerBrush( QBrush const& brush );
   //! \brief Set the text displayed above the marker.
   void setMarkerText( QString const& text );
   //! \brief If true, the marker text will always be updated to the value given by \c setValue().
   void setMarkerTextIsValue(bool val);
   
   /*!
    * \brief Set the tick mark intervals.
    * 
    * If either parameter is <= 0, then the tick marks are not drawn.
    * 
    * \param primaryInterval How often to draw big tick marks.
    * \param secondaryTicks Number of secondary ticks per primary tick.
    */
   void setTickMarks( double primaryInterval, int secondaryTicks = 1 );
   
   //! \brief Set the \c precision for displaying values.
   void setPrecision(int precision);
   
   //! \brief Reimplemented from QWidget.
   virtual QSize sizeHint() const;

public slots:
   
   //! \brief Set the \c value for the indicator.
   void setValue(double value);

   /*!
    * \brief Set the range of values considered to be *best*
    * 
    * \param range \c range.first and \c range.second are the min and max
    *        values for the preferred range resp.
    */
   void setPreferredRange(QPair<double,double> range);
   
   /*!
    * \brief Set the range of values that the widget displays
    * 
    * \param range \c range.first and \c range.second are the min and max
    *        values for the preferred range resp.
    */
   void setRange(QPair<double,double> range);
   
   //! \brief Convenience method for setting the widget range
   void setRange( double min, double max );   
   
   //! \brief Convenience method for setting the preferred range
   void setPreferredRange( double min, double max );
   
protected:
   //! \brief Reimplemented from QWidget.
   virtual void paintEvent(QPaintEvent* event);
   //! \brief Reimplemented from QWidget for popup on mouseover.
   virtual void mouseMoveEvent(QMouseEvent* event);
   
private:
   double _min;
   double _max;
   double _prefMin;
   double _prefMax;
   double _val;
   QString _valText;
   QString _markerText;
   int _prec;
   double _tickInterval;
   int _secondaryTicks;
   QString _tooltipText;
   QBrush _bgBrush;
   QBrush _prefRangeBrush;
   QPen _prefRangePen;
   QBrush _markerBrush;
   bool _markerTextIsValue;
   int _pixelRatio;
};

#endif /*RANGEDSLIDER_H*/
