#include <instruments.h>

sg_instrument_t instr_lead1 = {
    .a = 50,
    .d = 100,
    .s = 100,
    .r = 200,
    .amp_base = 0.7f,
    .osc_func = gen_func_square,
};

sg_instrument_t instr_lead1_long = {
    .a = 250,
    .d = 1000,
    .s = 500,
    .r = 500,
    .amp_base = 0.7f,
    .osc_func = gen_func_sawtooth,
};

sg_instrument_t instr_bass1 = {
	.a = 50,
    .d = 100,
    .s = 150,
    .r = 300,
    .amp_base = 0.4f,
    .osc_func = gen_func_sawtooth,
};

sg_instrument_t instr_drum_snare = {
	.a = 30,
    .d = 100,
    .s = 0,
    .r = 120,
    .amp_base = 0.3f,
    .osc_func = gen_func_noise,
};