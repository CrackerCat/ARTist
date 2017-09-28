/**
 * The ARTist Project (https://artist.cispa.saarland)
 *
 * Copyright (C) 2017 CISPA (https://cispa.saarland), Saarland University
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author "Oliver Schranz <oliver.schranz@cispa.saarland>"
 * @author "Sebastian Weisgerber <weisgerber@cispa.saarland>"
 *
 */

#include "optimizing/artist/api/env/java_env.h"
#include "method_info.h"
#include "utils.h"
#include "optimizing/artist/api/utils/artist_utils.h"

namespace art {

  ArtistMethodInfo::ArtistMethodInfo(
    HGraph* methodGraph,
    const DexCompilationUnit& compUnit )
    : graph(methodGraph)
    , compilationUnit(compUnit)
#ifdef BUILD_OREO
    , methodName(graph->GetDexFile().PrettyMethod(graph->GetMethodIdx(), false))
    , methodNameWithSignature(graph->GetDexFile().PrettyMethod(graph->GetMethodIdx(), true)) {
#else
    , methodName(PrettyMethod(graph->GetMethodIdx(), graph->GetDexFile(), false))
    , methodNameWithSignature(PrettyMethod(graph->GetMethodIdx(), graph->GetDexFile(), true)) {
#endif
    // the other field will be set in the factory
  }

  const string& ArtistMethodInfo::GetMethodName(bool signature) const {
    if (signature) {
      return methodNameWithSignature;
    };
    return methodName;
  }

  bool ArtistMethodInfo::IsStatic() const {
    return compilationUnit.IsStatic();
  }

  const vector<HParameterValue*>& ArtistMethodInfo::GetParams() const {
    return params;
  }

  const vector<string>& ArtistMethodInfo::GetParamTypes() const {
    return paramTypes;
  }

  bool ArtistMethodInfo::IsStringParam(HParameterValue* param) const {
    DCHECK(param != nullptr);

    int arg_index = param->GetIndex();

    // TODO decrement if static method
    if (IsStatic()) {
      arg_index--;
    }
    // non-static method with no other arguments than this:
    // this is argument 0, we substract 1 and get -1. So we have to return before trying to access the -1th arg type.
    if (arg_index < 0) {
      return false;
    }
    return paramTypes[arg_index].compare(JavaEnvironment::C_STRING) == 0;
  }

  bool ArtistMethodInfo::IsThisParameter(HParameterValue* param) const {
    // CanBeNull is implemented as !is_this
    return !param->CanBeNull();
  }


HGraph* ArtistMethodInfo::GetGraph() const {
  return graph;
}

ostream &operator<<(ostream &os, const ArtistMethodInfo &info) {
  os << "ArtistMethodInfo { method: ";
  if (info.IsStatic()) {
    os << "static ";
  }
  os << info.GetMethodName(true);


  os << "}";
  return os;
}

}  // namespace art
