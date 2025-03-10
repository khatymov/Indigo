/****************************************************************************
 * Copyright (C) from 2009 to Present EPAM Systems.
 *
 * This file is part of Indigo toolkit.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ***************************************************************************/

#ifndef __molecule_json_saver_h__
#define __molecule_json_saver_h__

#include <sstream>

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "base_cpp/locale_guard.h"
#include "base_cpp/output.h"
#include "molecule/base_molecule.h"
#include "molecule/elements.h"
#include "molecule/query_molecule.h"

namespace indigo
{

    class Molecule;
    class QueryMolecule;
    class Output;

    class DLLEXPORT JsonWriter
    {
    public:
        typedef rapidjson::Writer<rapidjson::StringBuffer>::Ch Ch;

        explicit JsonWriter(bool is_pretty = false) : pretty_json(is_pretty)
        {
        }

        void Reset(rapidjson::StringBuffer& os)
        {
            if (pretty_json)
                _pretty_writer.Reset(os);
            else
                _writer.Reset(os);
        }

        bool IsComplete() const
        {
            return pretty_json ? _pretty_writer.IsComplete() : _writer.IsComplete();
        }

        int GetMaxDecimalPlaces() const
        {
            return pretty_json ? _pretty_writer.GetMaxDecimalPlaces() : _writer.GetMaxDecimalPlaces();
        }

        void SetMaxDecimalPlaces(int maxDecimalPlaces)
        {
            if (pretty_json)
                _pretty_writer.SetMaxDecimalPlaces(maxDecimalPlaces);
            else
                _writer.SetMaxDecimalPlaces(maxDecimalPlaces);
        }

        bool Null()
        {
            return pretty_json ? _pretty_writer.Null() : _writer.Null();
        }

        bool Bool(bool b)
        {
            return pretty_json ? _pretty_writer.Bool(b) : _writer.Bool(b);
        }

        bool Int(int i)
        {
            return pretty_json ? _pretty_writer.Int(i) : _writer.Int(i);
        }

        bool Uint(unsigned u)
        {
            return pretty_json ? _pretty_writer.Uint(u) : _writer.Uint(u);
        }

        bool Int64(int64_t i64)
        {
            return pretty_json ? _pretty_writer.Uint64(i64) : _writer.Uint64(i64);
        }

        bool Uint64(uint64_t u64)
        {
            return pretty_json ? _pretty_writer.Uint64(u64) : _writer.Uint64(u64);
        }

        bool Double(double d)
        {
            return pretty_json ? _pretty_writer.Double(d) : _writer.Double(d);
        }

        bool RawNumber(const Ch* str, rapidjson::SizeType length, bool copy = false)
        {
            return pretty_json ? _pretty_writer.RawNumber(str, length, copy) : _writer.RawNumber(str, length, copy);
        }

        bool String(const Ch* str, rapidjson::SizeType length, bool copy = false)
        {
            return pretty_json ? _pretty_writer.String(str, length, copy) : _writer.String(str, length, copy);
        }

        bool StartObject()
        {
            return pretty_json ? _pretty_writer.StartObject() : _writer.StartObject();
        }

        bool Key(const Ch* str, rapidjson::SizeType length, bool copy = false)
        {
            return pretty_json ? _pretty_writer.Key(str, length, copy) : _writer.Key(str, length, copy);
        }

        bool EndObject(rapidjson::SizeType memberCount = 0)
        {
            return pretty_json ? _pretty_writer.EndObject(memberCount) : _writer.EndObject(memberCount);
        }

        bool StartArray()
        {
            return pretty_json ? _pretty_writer.StartArray() : _writer.StartArray();
        }

        bool EndArray(rapidjson::SizeType elementCount = 0)
        {
            return pretty_json ? _pretty_writer.EndArray(elementCount) : _writer.EndArray(elementCount);
        }

        bool String(const Ch* const& str)
        {
            return pretty_json ? _pretty_writer.String(str) : _writer.String(str);
        }

        bool Key(const Ch* const& str)
        {
            return pretty_json ? _pretty_writer.Key(str) : _writer.Key(str);
        }

        bool RawValue(const Ch* json, size_t length, rapidjson::Type type)
        {
            return pretty_json ? _pretty_writer.RawValue(json, length, type) : _writer.RawValue(json, length, type);
        }

        void Flush()
        {
            if (pretty_json)
                _pretty_writer.Flush();
            else
                _writer.Flush();
        }

    private:
        bool pretty_json;
        rapidjson::Writer<rapidjson::StringBuffer> _writer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> _pretty_writer;
    };

    class DLLEXPORT MoleculeJsonSaver
    {
    public:
        explicit MoleculeJsonSaver(Output& output);
        void saveMolecule(BaseMolecule& bmol);
        void saveMolecule(BaseMolecule& bmol, JsonWriter& writer);
        static void saveMetaData(JsonWriter& writer, MetaDataStorage& meta);
        bool add_stereo_desc;
        bool pretty_json;

    protected:
        void saveAtoms(BaseMolecule& mol, JsonWriter& writer);
        void saveBonds(BaseMolecule& mol, JsonWriter& writer);
        void saveRGroup(PtrPool<BaseMolecule>& fragments, int rgnum, JsonWriter& writer);
        void saveFragment(BaseMolecule& fragment, JsonWriter& writer);
        void saveTGroup(TGroup& tg, JsonWriter& writer);

        void saveSGroups(BaseMolecule& mol, JsonWriter& writer);
        void saveSGroup(SGroup& sgroup, JsonWriter& writer);
        void saveAttachmentPoint(BaseMolecule& mol, int atom_idx, JsonWriter& writer);
        void saveStereoCenter(BaseMolecule& mol, int atom_idx, JsonWriter& writer);
        void saveHighlights(BaseMolecule& mol, JsonWriter& writer);
        void saveSelection(BaseMolecule& mol, JsonWriter& writer);

        DECL_ERROR;

    protected:
        void _checkSGroupIndices(BaseMolecule& mol, Array<int>& sgs_list);
        bool _checkAttPointOrder(BaseMolecule& mol, int rsite);
        bool _needCustomQuery(QueryMolecule::Atom* atom) const;
        void _writeQueryProperties(QueryMolecule::Atom* atom, JsonWriter& writer);

        Molecule* _pmol;
        QueryMolecule* _pqmol;
        Output& _output;

    private:
        MoleculeJsonSaver(const MoleculeJsonSaver&); // no implicit copy
    };

} // namespace indigo

#endif
