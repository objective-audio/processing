//
//  yas_processing_track_chain_tests.mm
//

#import <XCTest/XCTest.h>
#import <processing/yas_processing_umbrella.h>

using namespace yas;

@interface yas_processing_track_chain_tests : XCTestCase

@end

@implementation yas_processing_track_chain_tests

- (void)setUp {
}

- (void)tearDown {
}

- (void)test_fetched {
    proc::track track;

    proc::module module1{proc::module::processors_t{}};
    proc::module module2{proc::module::processors_t{}};
    track.insert_module({0, 1}, module1);
    track.insert_module({1, 1}, module2);

    std::vector<proc::track::event_t> events;

    auto chain = track.chain().perform([&events](auto const &event) { events.push_back(event); }).sync();

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), chaining::event_type::fetched);
    auto iterator = events.at(0).get<proc::track::fetched_event_t>().elements.begin();
    XCTAssertEqual(iterator->first, (proc::time::range{0, 1}));
    XCTAssertEqual(iterator->second, module1);
    ++iterator;
    XCTAssertEqual(iterator->first, (proc::time::range{1, 1}));
    XCTAssertEqual(iterator->second, module2);
}

- (void)test_inserted {
    proc::track track;

    std::vector<proc::track::event_t> events;
    std::vector<std::multimap<proc::time::range, proc::module>> inserted;

    auto chain = track.chain()
                     .perform([&events, &inserted](auto const &event) {
                         events.push_back(event);
                         inserted.push_back(event.template get<proc::track::inserted_event_t>().elements);
                     })
                     .end();

    proc::module module1{proc::module::processors_t{}};
    proc::module module2{proc::module::processors_t{}};

    track.insert_module({0, 1}, module1);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), chaining::event_type::inserted);
    XCTAssertEqual(inserted.size(), 1);
    XCTAssertEqual(inserted.at(0).size(), 1);
    XCTAssertEqual(inserted.at(0).begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(inserted.at(0).begin()->second, module1);

    track.insert_module({1, 1}, module2);

    XCTAssertEqual(events.size(), 2);
    XCTAssertEqual(events.at(1).type(), chaining::event_type::inserted);
    XCTAssertEqual(inserted.size(), 2);
    XCTAssertEqual(inserted.at(1).size(), 1);
    XCTAssertEqual(inserted.at(1).begin()->first, (proc::time::range{1, 1}));
    XCTAssertEqual(inserted.at(1).begin()->second, module2);
}

- (void)test_erased {
    proc::track track;

    proc::module module1{proc::module::processors_t{}};
    proc::module module2{proc::module::processors_t{}};
    track.insert_module({0, 1}, module1);
    track.insert_module({1, 1}, module2);

    std::vector<proc::track::event_t> events;
    std::vector<std::multimap<proc::time::range, proc::module>> erased;

    auto chain = track.chain()
                     .perform([&events, &erased](auto const &event) {
                         events.push_back(event);
                         if (event.type() == chaining::event_type::erased) {
                             erased.push_back(event.template get<proc::track::erased_event_t>().elements);
                         }
                     })
                     .end();

    track.erase_module(module1);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), chaining::event_type::erased);

    XCTAssertEqual(erased.size(), 1);
    XCTAssertEqual(erased.at(0).size(), 1);
    XCTAssertEqual(erased.at(0).begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(erased.at(0).begin()->second, module1);
}

@end
