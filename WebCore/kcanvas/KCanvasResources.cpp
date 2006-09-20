/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>

    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "config.h"
#ifdef SVG_SUPPORT
#include "KCanvasResources.h"

#include "Document.h"
#include "GraphicsContext.h"
#include "IntRect.h"
#include "RenderSVGContainer.h"
#include "KCanvasImage.h"
#include "KCanvasTreeDebug.h"
#include "KRenderingDevice.h"
#include "RenderPath.h"
#include "SVGStyledElement.h"
#include "TextStream.h"

namespace WebCore {

TextStream &operator<<(TextStream& ts, const KCanvasResource& r) 
{ 
    return r.externalRepresentation(ts); 
}

//WindRule
TextStream &operator<<(TextStream &ts, WindRule rule)
{
    switch (rule) 
    {
        case RULE_NONZERO:
            ts << "NON-ZERO"; break;
        case RULE_EVENODD:
            ts << "EVEN-ODD"; break;
    }
    return ts;
}

//KCClipData
TextStream &operator<<(TextStream &ts, const KCClipData &d)
{
    ts << "[winding=" << d.windRule() << "]";
    if (d.bboxUnits)
        ts << " [bounding box mode=" << d.bboxUnits  << "]";    
    ts << " [path=" << d.path.debugString() << "]";
    return ts;
}

// KCanvasResource
KCanvasResource::KCanvasResource()
{
}

KCanvasResource::~KCanvasResource()
{
}

void KCanvasResource::addClient(const RenderPath *item)
{
    if(m_clients.find(item) != m_clients.end())
        return;

    m_clients.append(item);
}

const RenderPathList &KCanvasResource::clients() const
{
    return m_clients;
}

void KCanvasResource::invalidate()
{
    RenderPathList::ConstIterator it = m_clients.begin();
    RenderPathList::ConstIterator end = m_clients.end();

    for(; it != end; ++it)
        const_cast<RenderPath *>(*it)->repaint();
}

String KCanvasResource::idInRegistry() const
{
    return m_registryId;
}

void KCanvasResource::setIdInRegistry(const String& newId)
{
    m_registryId = newId;
} 

TextStream& KCanvasResource::externalRepresentation(TextStream &ts) const
{
    return ts;
}

// KCanvasClipper
KCanvasClipper::KCanvasClipper() : KCanvasResource()
{
}

KCanvasClipper::~KCanvasClipper()
{
}

void KCanvasClipper::resetClipData()
{
    m_clipData.clear();
}

void KCanvasClipper::addClipData(const Path& path, WindRule rule, bool bboxUnits)
{
    m_clipData.addPath(path, rule, bboxUnits);
}

KCClipDataList KCanvasClipper::clipData() const
{
    return m_clipData;
}

TextStream& KCanvasClipper::externalRepresentation(TextStream &ts) const
{
    ts << "[type=CLIPPER]";
    ts << " [clip data=" << clipData() << "]";
    return ts;
}

// KCanvasMasker
KCanvasMasker::KCanvasMasker() : KCanvasResource(), m_mask(0)
{
}

KCanvasMasker::~KCanvasMasker()
{
    delete m_mask;
}

void KCanvasMasker::setMask(KCanvasImage *mask)
{
    if (m_mask != mask) {
        delete m_mask;
        m_mask = mask;
    }
}

TextStream& KCanvasMasker::externalRepresentation(TextStream &ts) const
{
    ts << "[type=MASKER]";
    return ts;
}

// KCanvasMarker
KCanvasMarker::KCanvasMarker(RenderSVGContainer* marker)
    : KCanvasResource()
    , m_refX(0)
    , m_refY(0)
    , m_marker(marker)
    , m_useStrokeWidth(true)
{
    setAutoAngle();
}

KCanvasMarker::~KCanvasMarker()
{
}

void KCanvasMarker::setMarker(RenderSVGContainer* marker)
{
    m_marker = marker;
}

void KCanvasMarker::setRef(double refX, double refY)
{
    m_refX = refX;
    m_refY = refY;
}

double KCanvasMarker::refX() const
{
    return m_refX;
}

double KCanvasMarker::refY() const
{
    return m_refY;
}

void KCanvasMarker::setAngle(float angle)
{
    m_angle = angle;
}

float KCanvasMarker::angle() const
{
    return m_angle;
}

void KCanvasMarker::setAutoAngle()
{
    m_angle = -1;
}

void KCanvasMarker::setUseStrokeWidth(bool useStrokeWidth)
{
    m_useStrokeWidth = useStrokeWidth;
}

bool KCanvasMarker::useStrokeWidth() const
{
    return m_useStrokeWidth;
}

void KCanvasMarker::draw(GraphicsContext* context, const FloatRect& rect, double x, double y, double strokeWidth, double angle)
{
    if (!m_marker)
        return;
    
    AffineTransform transform;
    transform.translate(x, y);
    transform.rotate(m_angle > -1 ? m_angle : angle);
    
    // refX and refY are given in coordinates relative to the viewport established by the marker, yet they affect
    // the translation performed on the viewport itself.
    AffineTransform viewportTransform;
    if (m_useStrokeWidth)
        viewportTransform.scale(strokeWidth, strokeWidth);
    if (!m_marker->viewBox().isEmpty())
        viewportTransform *= m_marker->viewportTransform();
    double refX, refY;
    viewportTransform.map(m_refX, m_refY, &refX, &refY);
    transform.translate(-refX, -refY);
    
    if (m_useStrokeWidth)
        transform.scale(strokeWidth, strokeWidth);
    
    // FIXME: PaintInfo should be passed into this method instead of being created here
    // FIXME: bounding box fractions are lost
    RenderObject::PaintInfo info(context, enclosingIntRect(rect), PaintPhaseForeground, 0, 0, 0);
    
    context->save();
    context->concatCTM(transform);
    m_marker->setDrawsContents(true);
    m_marker->paint(info, 0, 0);
    m_marker->setDrawsContents(false);
    context->restore();
}

TextStream& KCanvasMarker::externalRepresentation(TextStream &ts) const
{
    ts << "[type=MARKER]"
       << " [angle=";
    if (angle() == -1) 
        ts << "auto" << "]";
    else 
        ts << angle() << "]";        
    ts << " [ref x=" << refX() << " y=" << refY() << "]";
    return ts;
}

KCanvasResource *getResourceById(Document *document, const AtomicString &id)
{
    if (id.isEmpty())
        return 0;
    Element *element = document->getElementById(id);
    SVGElement *svgElement = svg_dynamic_cast(element);
    if (svgElement && svgElement->isStyled())
        return static_cast<SVGStyledElement *>(svgElement)->canvasResource();
    return 0;
}

KCanvasMarker *getMarkerById(Document *document, const AtomicString &id)
{
    KCanvasResource *resource = getResourceById(document, id);
    if (resource && resource->isMarker())
        return static_cast<KCanvasMarker *>(resource);
    return 0;
}

KCanvasClipper *getClipperById(Document *document, const AtomicString &id)
{
    KCanvasResource *resource = getResourceById(document, id);
    if (resource && resource->isClipper())
        return static_cast<KCanvasClipper *>(resource);
    return 0;
}

KCanvasMasker *getMaskerById(Document *document, const AtomicString &id)
{
    KCanvasResource *resource = getResourceById(document, id);
    if (resource && resource->isMasker())
        return static_cast<KCanvasMasker *>(resource);
    return 0;
}

KRenderingPaintServer *getPaintServerById(Document *document, const AtomicString &id)
{
    KCanvasResource *resource = getResourceById(document, id);
    if (resource && resource->isPaintServer())
        return static_cast<KRenderingPaintServer *>(resource);
    return 0;
}

}

// vim:ts=4:noet
#endif // SVG_SUPPORT

