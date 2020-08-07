/* Copyright 2016, Ableton AG, Berlin. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like to incorporate Link into a proprietary software application,
 *  please contact <link-devs@ableton.com>.
 */

#pragma once

#include <ableton/discovery/NetworkByteStreamSerializable.hpp>
#include <cmath>
#include <cstdint>

namespace ableton
{
namespace link
{

struct Beats
{
  constexpr Beats() = default;

  explicit Beats(const double beats) noexcept
    : mValue(std::llround(beats * 1e6))
  {
  }

  explicit constexpr Beats(const std::int64_t microBeats) noexcept
    : mValue(microBeats)
  {
  }

  constexpr double floating() const noexcept
  {
    return microBeats() / 1e6;
  }

  constexpr std::int64_t microBeats() const noexcept
  {
    return mValue;
  }

  constexpr Beats operator-() const noexcept
  {
    return Beats{-microBeats()};
  }

  friend Beats abs(const Beats b) noexcept
  {
    return Beats{std::abs(b.microBeats())};
  }

  friend constexpr Beats operator+(const Beats lhs, const Beats rhs) noexcept
  {
    return Beats{lhs.microBeats() + rhs.microBeats()};
  }

  friend constexpr Beats operator-(const Beats lhs, const Beats rhs) noexcept
  {
    return Beats{lhs.microBeats() - rhs.microBeats()};
  }

  friend constexpr Beats operator%(const Beats lhs, const Beats rhs) noexcept
  {
    return Beats{rhs.mValue == 0 ? 0 : (lhs.mValue % rhs.mValue)};
  }

  friend constexpr bool operator<(const Beats lhs, const Beats rhs) noexcept
  {
    return lhs.mValue < rhs.mValue;
  }

  friend constexpr bool operator>(const Beats lhs, const Beats rhs) noexcept
  {
    return lhs.mValue > rhs.mValue;
  }

  friend constexpr bool operator==(const Beats lhs, const Beats rhs) noexcept
  {
    return lhs.mValue == rhs.mValue;
  }

  friend constexpr bool operator!=(const Beats lhs, const Beats rhs) noexcept
  {
    return lhs.mValue != rhs.mValue;
  }

  // Model the NetworkByteStreamSerializable concept
  friend std::uint32_t sizeInByteStream(const Beats beats)
  {
    return discovery::sizeInByteStream(beats.microBeats());
  }

  template <typename It>
  friend It toNetworkByteStream(const Beats beats, It out)
  {
    return discovery::toNetworkByteStream(beats.microBeats(), std::move(out));
  }

  template <typename It>
  static std::pair<Beats, It> fromNetworkByteStream(It begin, It end)
  {
    auto result = discovery::Deserialize<std::int64_t>::fromNetworkByteStream(
      std::move(begin), std::move(end));
    return std::make_pair(Beats{result.first}, std::move(result.second));
  }

private:
    std::int64_t mValue = 0;
};

} // namespace link
} // namespace ableton
