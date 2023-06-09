/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "distributed_sched_continuation_test.h"
#include "dtbschedmgr_device_info_storage.h"
#include "mock_distributed_sched.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS::AAFwk;
using namespace OHOS::AppExecFwk;
using string = std::string;

namespace OHOS {
namespace DistributedSchedule {
namespace {
const std::u16string MOCK_DEVICE_ID = u"MOCK_DEVICE_ID";
constexpr int32_t MOCK_SESSION_ID = 123;
const string LOCAL_DEVICE_ID = "192.168.43.100";
}

void DSchedContinuationTest::SetUpTestCase()
{
}

void DSchedContinuationTest::TearDownTestCase()
{
}

void DSchedContinuationTest::SetUp()
{
    dschedContinuation_ = std::make_shared<DSchedContinuation>();
}

void DSchedContinuationTest::TearDown()
{
    dschedContinuation_ = nullptr;
}

sptr<IRemoteObject> DSchedContinuationTest::GetDSchedService() const
{
    sptr<IRemoteObject> dsched = new MockDistributedSched();
    return dsched;
}

int32_t DSchedContinuationTest::PushAbilityToken()
{
    FuncContinuationCallback continuationCallback = [this] (int32_t missionId) {
        timeoutFlag_ = true;
    };
    dschedContinuation_->Init(continuationCallback);
    int32_t sessionId = dschedContinuation_->GenerateSessionId();
    dschedContinuation_->PushAbilityToken(sessionId, GetDSchedService());
    return sessionId;
}

std::shared_ptr<Want> DSchedContinuationTest::MockWant(const string& bundleName, const string& ability, int32_t flags)
{
    ElementName element("", bundleName, ability);
    std::shared_ptr<Want> spWant = std::make_shared<Want>();
    spWant->SetElement(element);
    spWant->SetFlags(flags);
    return spWant;
}

int32_t DSchedContinuationTest::StartContinuation(int32_t missionId, int32_t flags)
{
    string bundleName = "bundleName";
    string abilityName = "abilityName";
    string devId = "devId";
    std::shared_ptr<Want> spWant = MockWant(bundleName, abilityName, flags);
    int callerUid = 0;
    return DistributedSchedService::GetInstance().StartContinuation(*spWant, missionId, callerUid, 0, 0);
}

/**
 * @tc.name: StartContinuation_001
 * @tc.desc: input invalid params.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, StartContinuation_001, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest StartContinuation_001 start" << std::endl;
    /**
     * @tc.steps: step1. want not set continuation flags.
     * @tc.expected: step1. return false.
     */
    int32_t ret = StartContinuation(0, 0);
    EXPECT_TRUE(ret != ERR_OK);
    DTEST_LOG << "DSchedContinuationTest StartContinuation002 end" << std::endl;
}

/**
 * @tc.name: StartContinuation_002
 * @tc.desc: get remote dms failed.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, StartContinuation_002, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest StartContinuation_002 start" << std::endl;
    /**
     * @tc.steps: step1. get remote dms failed.
     * @tc.expected: step1. return false.
     */
    int32_t ret = StartContinuation(0, Want::FLAG_ABILITY_CONTINUATION);
    EXPECT_TRUE(ret != ERR_OK);
    DTEST_LOG << "DSchedContinuationTest StartContinuation003 end" << std::endl;
}

/**
 * @tc.name: NotifyCompleteContinuation_001
 * @tc.desc: input invalid session.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, NotifyCompleteContinuation_001, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest NotifyCompleteContinuation_001 start" << std::endl;
    /**
     * @tc.steps: step1. input invalid session.
     * @tc.expected: step1. return false.
     */
    DistributedSchedService::GetInstance().NotifyCompleteContinuation(MOCK_DEVICE_ID, -1, true);
    EXPECT_TRUE(!timeoutFlag_);
    DTEST_LOG << "DSchedContinuationTest NotifyCompleteContinuation_001 end" << std::endl;
}

/**
 * @tc.name: NotifyCompleteContinuation_002
 * @tc.desc: get remote dms failed.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, NotifyCompleteContinuation_002, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest NotifyCompleteContinuation_002 start" << std::endl;
    /**
     * @tc.steps: step1. get remote dms failed.
     * @tc.expected: step1. return false.
     */
    DistributedSchedService::GetInstance().NotifyCompleteContinuation(MOCK_DEVICE_ID, MOCK_SESSION_ID, true);
    EXPECT_TRUE(!timeoutFlag_);
    DTEST_LOG << "DSchedContinuationTest NotifyCompleteContinuation_002 end" << std::endl;
}

/**
 * @tc.name: NotifyContinuationResultFromRemote_001
 * @tc.desc: input invalid session.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, NotifyContinuationResultFromRemote_001, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest NotifyContinuationResultFromRemote_001 start" << std::endl;
    /**
     * @tc.steps: step1. input invalid session.
     * @tc.expected: step1. return false.
     */
    DistributedSchedService::GetInstance().NotifyContinuationResultFromRemote(-1, true);
    EXPECT_TRUE(!timeoutFlag_);
    DTEST_LOG << "DSchedContinuationTest NotifyContinuationResultFromRemote_001 end" << std::endl;
}

/**
 * @tc.name: NotifyContinuationResultFromRemote_002
 * @tc.desc: get remote dms failed.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, NotifyContinuationResultFromRemote_002, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest NotifyContinuationResultFromRemote_002 start" << std::endl;
    /**
     * @tc.steps: step1. get remote dms failed.
     * @tc.expected: step1. return false.
     */
    DistributedSchedService::GetInstance().NotifyContinuationResultFromRemote(MOCK_SESSION_ID, true);
    EXPECT_TRUE(!timeoutFlag_);
    DTEST_LOG << "DSchedContinuationTest NotifyContinuationResultFromRemote_002 end" << std::endl;
}

/**
 * @tc.name: PushAbilityToken_001
 * @tc.desc: input invalid params.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, PushAbilityToken_001, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest PushAbilityToken_001 start" << std::endl;
    /**
     * @tc.steps: step1. input invalid abilityToken.
     * @tc.expected: step1. return false.
     */
    auto sessionId = dschedContinuation_->GenerateSessionId();
    bool ret = dschedContinuation_->PushAbilityToken(sessionId, nullptr);
    EXPECT_TRUE(!ret);
    DTEST_LOG << "DSchedContinuationTest PushAbilityToken_001 end" << std::endl;
}

/**
 * @tc.name: PushAbilityToken_002
 * @tc.desc: input invalid params.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, PushAbilityToken_002, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest PushAbilityToken_002 start" << std::endl;
    /**
     * @tc.steps: step1. input invalid sessionId.
     * @tc.expected: step1. return false.
     */
    bool ret = dschedContinuation_->PushAbilityToken(-1, GetDSchedService());
    EXPECT_TRUE(!ret);
    DTEST_LOG << "DSchedContinuationTest PushAbilityToken_002 end" << std::endl;
}

/**
 * @tc.name: PushAbilityToken_003
 * @tc.desc: init not call.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, PushAbilityToken_003, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest PushAbilityToken_003 start" << std::endl;
    /**
     * @tc.steps: step1. input valid abilityToken and valid sessionId.
     * @tc.expected: step1. return false.
     */
    auto sessionId = dschedContinuation_->GenerateSessionId();
    bool ret = dschedContinuation_->PushAbilityToken(sessionId, GetDSchedService());
    EXPECT_TRUE(!ret);
    DTEST_LOG << "DSchedContinuationTest PushAbilityToken_003 end" << std::endl;
}

/**
 * @tc.name: PushAbilityToken_004
 * @tc.desc: Push AbilityToken OK.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, PushAbilityToken_004, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest PushAbilityToken_004 start" << std::endl;
    /**
     * @tc.steps: step1. input valid params and init.
     * @tc.expected: step1. return true.
     */
    dschedContinuation_->Init(nullptr);
    auto sessionId = dschedContinuation_->GenerateSessionId();
    bool ret = dschedContinuation_->PushAbilityToken(sessionId, GetDSchedService());
    EXPECT_TRUE(ret);
    DTEST_LOG << "DSchedContinuationTest PushAbilityToken_004 end" << std::endl;
}

/**
 * @tc.name: PopAbilityToken_001
 * @tc.desc: input invalid params.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, PopAbilityToken_001, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest PopAbilityToken_001 start" << std::endl;
    /**
     * @tc.steps: step1. input invalid sessionId.
     * @tc.expected: step1. return false.
     */
    sptr<IRemoteObject> abilityToken = dschedContinuation_->PopAbilityToken(-1);
    EXPECT_TRUE(abilityToken == nullptr);
    DTEST_LOG << "DSchedContinuationTest PopAbilityToken_001 end" << std::endl;
}

/**
 * @tc.name: PopAbilityToken_002
 * @tc.desc: input invalid params.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, PopAbilityToken_002, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest PopAbilityToken_002 start" << std::endl;
    /**
     * @tc.steps: step1. pop not exist sessionId.
     * @tc.expected: step1. return false.
     */
    int32_t sessionId = dschedContinuation_->GenerateSessionId() + 1;
    sptr<IRemoteObject> abilityToken = dschedContinuation_->PopAbilityToken(sessionId);
    EXPECT_TRUE(abilityToken == nullptr);
    DTEST_LOG << "DSchedContinuationTest PopAbilityToken_002 end" << std::endl;
}

/**
 * @tc.name: PopAbilityToken_003
 * @tc.desc: pop abilityToken success.
 * @tc.type: FUNC
 */
HWTEST_F(DSchedContinuationTest, PopAbilityToken_003, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest PopAbilityToken_003 start" << std::endl;
    /**
     * @tc.steps: step1. pop exist sessionId.
     * @tc.expected: step1. return true.
     */
    int32_t sessionId =PushAbilityToken();
    sptr<IRemoteObject> abilityToken = dschedContinuation_->PopAbilityToken(sessionId);
    EXPECT_TRUE(abilityToken != nullptr);

    /**
     * @tc.steps: step2. duplicate pop abilityToken.
     * @tc.expected: step1. return false.
     */
    abilityToken = dschedContinuation_->PopAbilityToken(sessionId);
    EXPECT_TRUE(abilityToken == nullptr);
    DTEST_LOG << "DSchedContinuationTest PopAbilityToken_003 end" << std::endl;
}

/**
 * @tc.name: ContinueMission_001
 * @tc.desc: test ContinueMission when srcDeviceId is empty.
 * @tc.type: FUNC
 * @tc.require: SR000GKT4A
 */
HWTEST_F(DSchedContinuationTest, ContinueMission_001, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest ContinueMission_001 start" << std::endl;
    WantParams wantParams;
    int32_t ret = DistributedSchedService::GetInstance().ContinueMission("",
        "string", 1, GetDSchedService(), wantParams);
    EXPECT_TRUE(ret != ERR_OK);
    DTEST_LOG << "DSchedContinuationTest ContinueMission_001 end" << std::endl;
}

/**
 * @tc.name: ContinueMission_002
 * @tc.desc: test ContinueMission when dstDeviceId is empty.
 * @tc.type: FUNC
 * @tc.require: SR000GKT4A
 */
HWTEST_F(DSchedContinuationTest, ContinueMission_002, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest ContinueMission_002 start" << std::endl;
    WantParams wantParams;
    int32_t ret = DistributedSchedService::GetInstance().ContinueMission("string",
        "", 1, GetDSchedService(), wantParams);
    EXPECT_TRUE(ret != ERR_OK);
    DTEST_LOG << "DSchedContinuationTest ContinueMission_002 end" << std::endl;
}

/**
 * @tc.name: ContinueMission_003
 * @tc.desc: test ContinueMission when callback is nullptr.
 * @tc.type: FUNC
 * @tc.require: SR000GKT4A
 */
HWTEST_F(DSchedContinuationTest, ContinueMission_003, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest ContinueMission_003 start" << std::endl;
    WantParams wantParams;
    int32_t ret = DistributedSchedService::GetInstance().ContinueMission("string", "string", 1, nullptr, wantParams);
    EXPECT_TRUE(ret != ERR_OK);
    DTEST_LOG << "DSchedContinuationTest ContinueMission_003 end" << std::endl;
}

/**
 * @tc.name: ContinueMission_004
 * @tc.desc: test ContinueMission when srcDeviceId == localDevId.
 * @tc.type: FUNC
 * @tc.require: SR000GKT4A
 */
HWTEST_F(DSchedContinuationTest, ContinueMission_004, TestSize.Level1)
{
    DTEST_LOG << "DSchedContinuationTest ContinueMission_004 start" << std::endl;
    WantParams wantParams;

    std::string srcDeviceId;
    DtbschedmgrDeviceInfoStorage::GetInstance().GetLocalDeviceId(srcDeviceId);
    int32_t ret = DistributedSchedService::GetInstance().ContinueMission(srcDeviceId,
        "string", 1, GetDSchedService(), wantParams);
    EXPECT_TRUE(ret != ERR_OK);
    DTEST_LOG << "DSchedContinuationTest ContinueMission_004 end" << std::endl;
}
} // DistributedSchedule
} // namespace OHOS
