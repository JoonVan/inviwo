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

#ifndef IVW_INDEXLISTPROPERTY_H
#define IVW_INDEXLISTPROPERTY_H

#include <modules/brushingandlinking/brushingandlinkingmoduledefine.h>
#include <inviwo/core/common/inviwo.h>

#include <inviwo/core/properties/property.h>

namespace inviwo {

/**
 * \class IndexListProperty
 * \brief VERY_BRIEFLY_DESCRIBE_THE_CLASS
 * DESCRIBE_THE_CLASS
 */
class IVW_MODULE_BRUSHINGANDLINKING_API IndexListProperty : public Property{ 
public:
    InviwoPropertyInfo();

    IndexListProperty(std::string identifier, std::string displayName,
        InvalidationLevel invalidationLevel = InvalidationLevel::InvalidOutput);

    virtual ~IndexListProperty();

    void clear();


    virtual void set(const Property* src) override;

    bool has(size_t i) const;
    void insert(size_t i);
    void remove(size_t i);
    void set(const std::unordered_set<size_t> &indices);
    size_t numberOfBrushedIndices()const;

private:
    std::unordered_set<size_t> brushedIndices_;

};

} // namespace

#endif // IVW_INDEXLISTPROPERTY_H

