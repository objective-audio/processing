// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "processing",
    platforms: [.macOS(.v10_15), .iOS(.v13), .macCatalyst(.v13)],
    products: [
        .library(
            name: "processing",
            targets: ["processing"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/objective-audio/audio_engine.git", branch: "master"),
    ],
    targets: [
        .target(
            name: "processing",
            dependencies: [
                .product(name: "audio", package: "audio_engine")
            ],
            cSettings: [
                .unsafeFlags(["-fmodules"]),
            ]
        ),
        .testTarget(
            name: "processing-tests",
            dependencies: [
                "processing",
            ],
            cxxSettings: [
                .unsafeFlags(["-fcxx-modules"]),
            ]
        ),
    ],
    cLanguageStandard: .gnu18,
    cxxLanguageStandard: .gnucxx2b
)
