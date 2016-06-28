/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2016 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <modules/brushingandlinking/properties/brushinglistproperty.h>

namespace inviwo {
PropertyClassIdentifier(BrushingListProperty, "org.inviwo.BrushingListProperty");

BrushingListProperty::BrushingListProperty(
    std::string identifier, std::string displayName,
    InvalidationLevel invalidationLevel /*= InvalidationLevel::InvalidOutput*/)
    : Property(identifier, displayName, invalidationLevel) {}

BrushingListProperty::~BrushingListProperty() { clear(); }

void BrushingListProperty::clear() { setBrushed(std::unordered_set<size_t>()); }

void BrushingListProperty::set(const Property* src) {
    if (auto bList = dynamic_cast<const BrushingListProperty*>(src)) {
        setBrushed(bList->brushedIndices_);
    }
    Property::set(src);
}

bool BrushingListProperty::isBrushed(size_t i) const {
    if (brushedIndices_.find(i) != brushedIndices_.end()) {
        return true;
    }
    return false;
}

void BrushingListProperty::setBrushed(const std::unordered_set<size_t>& indices) {
    brushedIndices_ = indices;
    propertyModified();
}

void BrushingListProperty::setBrushed(size_t i) { brushedIndices_.insert(i); propertyModified(); }

void BrushingListProperty::setUnbrushed(size_t i) { brushedIndices_.erase(i);  propertyModified(); }

size_t BrushingListProperty::numberOfBrushedIndices() const { return brushedIndices_.size(); }

}  // namespace
