/*
 * Copyright 2020 - 2021 Maximilian Voss (maximilian@voss.rocks)
 *
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

#ifndef DEFINITIONS_INPUT_H
#define DEFINITIONS_INPUT_H

typedef struct {
    uint8_t up;
    uint8_t down;
    uint8_t left;
    uint8_t right;
    uint8_t start;
    uint8_t back;
    uint8_t leftShoulder;
    uint8_t rightShoulder;
    uint8_t leftStick;
    uint8_t rightStick;
    uint8_t buttonA;
    uint8_t buttonB;
    uint8_t buttonX;
    uint8_t buttonY;

    int16_t stickX;
    int16_t stickY;
} GameControllerState_t;

#endif
