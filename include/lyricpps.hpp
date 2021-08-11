// MIT License
//
// Copyright (c) 2021 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
// lyric++s - song lyrics engine interpreter
// -----------------------------------------
// lyric++s uses tree to store datas instead of plain-text parsing.
//
// github.com/ferhatgec/lyricpps
//
// an example:
//
// from: 0.0 to: 0.19 {(
//    A rat bit my sister Nell%
//    With Whitey on the moon%
//    Her face and arms began to swell%
//    And Whitey's on the moon
//  )}
//
// from: 0.20 to: 0.33 {(
//     I can't pay no doctor bills%
//     But Whitey's on the moon%
//     Ten years from now%
//     I'll be payin' still%
//     While Whitey's on the moon%
//     You know the man%
//     Just upped my rent last night%
//     Whitey's on the moon%
// )}
//
// from: 0.34 to: 0.49 {(
//     No hot water, no toilets, no lights%
//     But Whitey's on the moon%
//     I wonder why he's uppin' me?%
//     'Cause Whitey's on the moon%
//     I was already givin' him like 50 a week%
//     Whitey's on the moon
// )}
//
// from: 0.50 to: 1.04 {(
//     Taxes takin' my whole damn check%
//     Junkies makin' me a nervous wreck%
//     The price of food is goin' up%
//     And if all that crap wasn't enough%
//     A rat bit my sister Nell%
//     With Whitey on the moon
// )}
//
// from: 1.05 to: 1.47 {(
//     Her face and arms began to swell%
//     And Whitey's on the moon%
//     Was all that money I made last year%
//     For Whitey on the moon?%
//     How come I ain't got no money here?%
//     Hmm, Whitey on the moon%
//     You know I just 'bout had my fill%
//     Of Whitey on the moon%
//     I think I'll send these doctor bills%
//     Airmail special%
//     To Whitey on the moon
// )}

#ifndef LYRICPPS_LYRICPPS_HPP
#define LYRICPPS_LYRICPPS_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

enum LyricppsTokens {
    From,
    To,
    Data,
    End,
    Undef = -1
};

class lyricpps_tree {
public:
    lyricpps_tree* child;
    LyricppsTokens tok;
    std::string val;
    double start, end;

    lyricpps_tree() = default;
    ~lyricpps_tree() {
        delete child;
    }
};

class lyricpps {
    std::vector<lyricpps_tree> init;

    const std::vector<std::string> list {
        "from:",
        "to:",
        "{(",
        ")}"
    };
public:
    lyricpps() = default;
    ~lyricpps()= default;

    LyricppsTokens match(std::string data) noexcept {
        for(unsigned i = 0; i < this->list.size(); i++) {
            if(this->list[i] == data) {
                return static_cast<LyricppsTokens>(i);
            }
        }

        return Undef;
    }

    void parse(std::string __data) noexcept {
        std::vector<std::string> tok;
        std::string current, data;
        LyricppsTokens type = Undef;

        for(std::istringstream val(__data); val >> current;) {
            if(this->init.size() > 0) {
                switch(this->init.back().tok) {
                    case From: {
                        this->init.back().start = std::stod(current);
                        this->init.back().tok = Undef;
                        break;
                    }

                    case To: {
                        this->init.back().end = std::stod(current);
                        this->init.back().tok = Undef;
                        break;
                    }

                    case Data: {
                        if(this->match(current) == End) { break; }
                        data.append(current);

                        if(data.back() == '%') {
                            data.pop_back();
                            data.push_back('\n');
                        } else {
                            data.push_back(' ');
                        }

                        break;
                    }

                    case End: {
                        std::cout << data << '\n';
                        break;
                    }
                }
            }

            if(type == Data && this->match(current) != End) { continue; }

            type = this->match(current);

            switch(type) {
                case From: {
                    this->init.push_back(lyricpps_tree {
                        .tok = type
                    });
                    break;
                }

                case To: {
                    this->init.back().tok = type;
                    break;
                }

                case Data: {
                    this->init.back().tok = type;
                    break;
                }

                case End: {
                    this->init.back().val = data;
                    data.clear();
                    break;
                }
            }
        }
    }

    // you can also implement this
    // yourself to extract data that what do you want.
    void visit() noexcept {
        if(this->init.size() > 0) {
            auto last = this->init.back();

            for(auto& val : this->init) {
                std::cout << "start: " << val.start << '\n' <<
                "end: " << val.end << '\n' <<
                "data: " << val.val;

                if(val.start != last.start && val.end != last.end) {
                    std::cout << "\n---\n";
                }
            }
        }
    }
};

#endif // LYRICPPS_LYRICPPS_HPP