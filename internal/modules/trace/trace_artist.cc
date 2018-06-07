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

#include <atomic>

#include "trace_artist.h"
#include "trace_codelib.h"
#include "optimizing/artist/api/io/artist_log.h"
#include "optimizing/artist/internal/injection/injection_visitor.h"
#include "optimizing/artist/api/env/java_env.h"
#include "class_linker-inl.h"

#include "optimizing/artist/api/injection/primitives.h"

#include "optimizing/artist/api/io/verbose_printer.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;


namespace art {

vector<shared_ptr<const Injection>> HTraceArtist::ProvideInjections() const {
  VLOG(artistd) << "HTraceArtist::ProvideInjections()";

  vector<shared_ptr<const Parameter>> params;
  vector<shared_ptr<const Target>> targets;
  auto target = make_shared<const Target>(Target::GENERIC_TARGET, InjectionTarget::METHOD_END);
//  const Target target(".onCreate(", InjectionTarget::METHOD_START);
  targets.push_back(target);

  auto injection = make_shared<const Injection>(TraceCodeLib::TRACELOG, params, targets);

  vector<shared_ptr<const Injection>> results;
  results.push_back(injection);
  return results;
}

}  // namespace art