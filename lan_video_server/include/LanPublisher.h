// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/*!
 * @file LanPublisher.h
 * This header file contains the declaration of the publisher functions.
 *
 * This file was generated by the tool fastcdrgen.
 */

#ifndef _FAST_DDS_GENERATED_MYTEST_PUBLISHER_H_
#define _FAST_DDS_GENERATED_MYTEST_PUBLISHER_H_

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <thread>
#include <chrono>

using namespace eprosima::fastdds::dds;

// 模板元编程
template <class T, class TPubSubType>
class LanPublisher
{
public:
    LanPublisher()
        : participant_(nullptr), publisher_(nullptr), topic_(nullptr), writer_(nullptr), type_(new TPubSubType())
    {
    }

    // DomainParticipantFactory =》 participant_  =》 publisher_ =》 writer_
    //                                            =》 topic_
    virtual ~LanPublisher()
    {
        if (writer_ != nullptr)
        {
            publisher_->delete_datawriter(writer_);
        }
        if (publisher_ != nullptr)
        {
            participant_->delete_publisher(publisher_);
        }
        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    bool init(std::string topicName)
    {
        /* Initialize data_ here */

        // CREATE THE PARTICIPANT
        DomainParticipantQos pqos;
        pqos.name("Participant_pub");

        // 创建参与者
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);
        if (participant_ == nullptr)
        {
            return false;
        }

        // REGISTER THE TYPE
        type_.register_type(participant_);

        // CREATE THE PUBLISHER
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        if (publisher_ == nullptr)
        {
            return false;
        }
        
        std::cout << "publish topic name " << topicName << std::endl;
        // CREATE THE TOPIC
        topic_ = participant_->create_topic(
            topicName,
            type_.get_type_name(),
            TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr)
        {
            return false;
        }

        // CREATE THE WRITER
        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);
        if (writer_ == nullptr)
        {
            return false;
        }

        std::cout << "T DataWriter created." << std::endl;
        return true;
    }

    void run()
    {
        std::cout << "T DataWriter waiting for DataReaders." << std::endl;
        // Publication code
        while (true)
        {
            if (listener_.matched == 0)
            {
                std::cout << "waiting for DataReaders" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep 250 ms
            }
            else
            {
                std::cout << "write message" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));
                T st;
                writer_->write(&st);
            }
        }
    }

    void publish(T &st)
    {
        if (listener_.matched == 0) {
            std::cout << "there are no DataReaders" << std::endl;
        } else {
            // 实际传入是 void * 指针
            writer_->write(&st);
            std::cout << "write message" << std::endl;
        }
    }

private:
    eprosima::fastdds::dds::DomainParticipant *participant_;
    eprosima::fastdds::dds::Publisher *publisher_;
    eprosima::fastdds::dds::Topic *topic_;
    eprosima::fastdds::dds::DataWriter *writer_;
    eprosima::fastdds::dds::TypeSupport type_;

    class PubListener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:
        PubListener() = default;

        ~PubListener() override = default;

        virtual void on_publication_matched (
            eprosima::fastdds::dds::DataWriter *writer,
            const eprosima::fastdds::dds::PublicationMatchedStatus &info) override
        {
            if (info.current_count_change == 1)
            {
                matched = info.total_count;
                std::cout << "DataWriter matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                matched = info.total_count;
                std::cout << "DataWriter unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                          << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
            }
        }

        int matched = 0;
    } listener_;
};

#endif // _FAST_DDS_GENERATED_MYTEST_PUBLISHER_H_