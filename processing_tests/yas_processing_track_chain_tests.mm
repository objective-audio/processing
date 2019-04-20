//
//  yas_processing_track_chain_tests.mm
//

#import <XCTest/XCTest.h>
#import <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::proc;

@interface yas_processing_track_chain_tests : XCTestCase

@end

@implementation yas_processing_track_chain_tests

- (void)setUp {
}

- (void)tearDown {
}

- (void)test_fetched {
    proc::track track;

    proc::module module1{[] { return module::processors_t{}; }};
    proc::module module2{[] { return module::processors_t{}; }};
    track.insert_module({0, 1}, module1);
    track.insert_module({1, 1}, module2);

    std::vector<proc::track::event_t> events;

    auto chain = track.chain().perform([&events](auto const &event) { events.push_back(event); }).sync();

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), proc::track::event_type_t::fetched);
    auto iterator = events.at(0).get<proc::track::fetched_event_t>().elements.begin();
    XCTAssertEqual(iterator->first, (proc::time::range{0, 1}));
    XCTAssertEqual(iterator->second.size(), 1);
    XCTAssertEqual(iterator->second.at(0), module1);
    ++iterator;
    XCTAssertEqual(iterator->first, (proc::time::range{1, 1}));
    XCTAssertEqual(iterator->second.size(), 1);
    XCTAssertEqual(iterator->second.at(0), module2);
}

- (void)test_inserted {
    proc::track track;

    std::vector<proc::track::event_t> events;
    std::vector<std::map<proc::time::range, chaining::vector::holder<proc::module>>> inserted;

    auto chain = track.chain()
                     .perform([&events, &inserted](auto const &event) {
                         events.push_back(event);
                         inserted.push_back(event.template get<proc::track::inserted_event_t>().elements);
                     })
                     .end();

    proc::module module1{[] { return module::processors_t{}; }};
    proc::module module2{[] { return module::processors_t{}; }};

    track.insert_module({0, 1}, module1);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), proc::track::event_type_t::inserted);
    XCTAssertEqual(inserted.size(), 1);
    XCTAssertEqual(inserted.at(0).size(), 1);
    XCTAssertEqual(inserted.at(0).begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(inserted.at(0).begin()->second.size(), 1);
    XCTAssertEqual(inserted.at(0).begin()->second.at(0), module1);

    track.insert_module({1, 1}, module2);

    XCTAssertEqual(events.size(), 2);
    XCTAssertEqual(events.at(1).type(), proc::track::event_type_t::inserted);
    XCTAssertEqual(inserted.size(), 2);
    XCTAssertEqual(inserted.at(1).size(), 1);
    XCTAssertEqual(inserted.at(1).begin()->first, (proc::time::range{1, 1}));
    XCTAssertEqual(inserted.at(1).begin()->second.size(), 1);
    XCTAssertEqual(inserted.at(1).begin()->second.at(0), module2);
}

- (void)test_erased {
    proc::track track;

    proc::module module1{[] { return module::processors_t{}; }};
    proc::module module2{[] { return module::processors_t{}; }};
    track.insert_module({0, 1}, module1);
    track.insert_module({1, 1}, module2);

    std::vector<proc::track::event_t> events;
    std::vector<std::pair<proc::time::range, chaining::vector::holder<proc::module>>> relayed;
    std::vector<std::map<proc::time::range, chaining::vector::holder<proc::module>>> erased;

    auto chain = track.chain()
                     .perform([&events, &erased, &relayed](auto const &event) {
                         events.push_back(event);
                         if (event.type() == proc::track::event_type_t::relayed) {
                             auto const &relayed_event = event.template get<proc::track::relayed_event_t>();
                             auto const &key = relayed_event.key;
                             auto const &value = relayed_event.value;
                             relayed.push_back(std::make_pair(key, value));
                         } else if (event.type() == proc::track::event_type_t::erased) {
                             erased.push_back(event.template get<proc::track::erased_event_t>().elements);
                         }
                     })
                     .end();

    track.erase_module(module1);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), proc::track::event_type_t::erased);

    XCTAssertEqual(relayed.size(), 0);

    XCTAssertEqual(erased.size(), 1);
    XCTAssertEqual(erased.at(0).size(), 1);
    XCTAssertEqual(erased.at(0).begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(erased.at(0).begin()->second.size(), 1);
}

- (void)test_insert_and_erase_same_range {
    proc::track track;

    std::vector<proc::track::event_t> events;

    auto chain = track.chain().perform([&events](auto const &event) { events.push_back(event); }).end();

    proc::module module1{[] { return module::processors_t{}; }};
    proc::module module2{[] { return module::processors_t{}; }};

    track.insert_module({0, 1}, module1);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), proc::track::event_type_t::inserted);

    track.insert_module({0, 1}, module2);

    XCTAssertEqual(events.size(), 2);
    XCTAssertEqual(events.at(1).type(), proc::track::event_type_t::relayed);

    track.erase_module(module1);

    XCTAssertEqual(events.size(), 3);
    XCTAssertEqual(events.at(2).type(), proc::track::event_type_t::relayed);

    track.erase_module(module2);

    XCTAssertEqual(events.size(), 4);
    XCTAssertEqual(events.at(3).type(), proc::track::event_type_t::erased);
}

@end
