// Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "paddleaudio/src/pybind/kaldi/kaldi_feature_wrapper.h"

namespace paddleaudio {
namespace kaldi {

KaldiFeatureWrapper* KaldiFeatureWrapper::GetInstance() {
    static KaldiFeatureWrapper instance;
    return &instance;
}

bool KaldiFeatureWrapper::InitFbank(::kaldi::FbankOptions opts) {
    fbank_.reset(new Fbank(opts));
    return true;
}

py::array_t<float> KaldiFeatureWrapper::ComputeFbank(
    const py::array_t<float> wav) {
    py::buffer_info info = wav.request();
    ::kaldi::SubVector<::kaldi::BaseFloat> input_wav((float*)info.ptr, info.size);

    ::kaldi::Vector<::kaldi::BaseFloat> feats;
    bool flag = fbank_->ComputeFeature(input_wav, &feats);
    if (flag == false || feats.Dim() == 0) return py::array_t<float>();
    auto result = py::array_t<float>(feats.Dim());
    py::buffer_info xs = result.request();
    std::cout << std::endl;
    float* res_ptr = (float*)xs.ptr;
    for (int idx = 0; idx < feats.Dim(); ++idx) {
        *res_ptr = feats(idx);
        res_ptr++;
    }

    return result.reshape({feats.Dim() / Dim(), Dim()});
}

}  // namesapce kaldi
}  // namespace paddleaudio
