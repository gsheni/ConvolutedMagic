#include <stdio.h>
#include <stdlib.h>

typedef unsigned long u64;

/* Start here */
typedef void enc_cfg_t;
typedef int enc_cfg2_t;
typedef __int128_t dcf_t;

enc_cfg_t _ctx_iface(dcf_t s, enc_cfg2_t i){
	int c = (((s & ((dcf_t)0x1FULL << i * 5)) >> i * 5) + 65);
	printf("%c", c); }
	enc_cfg2_t main() {
	for (int i=0; i<17; i++){
		_ctx_iface(0x79481E6BBCC01223 + ((dcf_t)0x1222DC << 64), i);
	}
	printf("\n");
}
/* End here */

//TOneverDO change name
u64 HammingCtr(u64 a, u64 b) {
	u64 c = a ^ b;

	/*for (d = 0; c>0; c >>= 1)
	{
		d += c & 1;
	}*/
	// O(m) lol no thanks

	//1000 wrap into loop
	c = c - ((c >> 1) & ~0UL/3);
	c = (c & ~0UL/5) + ((c >> 2) & ~0UL/5);
	c = (c & 0UL/0x21) + ((c >> 4) & 0UL/0x11);
	c = (c & ~0UL/0x101) + ((c >> 8) & ~0UL/0x101);
	c = (c & ~0UL/0x10001)+((c>>16)&~0UL/0x10001);
	c = (c & ~0UL/0x100000001)+((c>>32)&~0UL/0x100000001);

	//TODO throw away intermediates... but could be useful later (see seander)
	return c;
}

//TODO transform + multiply spectra + transform back. faster? lossy?

u64 * ConvolutedMagic(u64 *x, u64 y, u64 *z, u64 n, u64 n_y) {
	//z is array of offsets in BITS
	//y is left-aligned of length n_y
	//TODO function is ridic fragile. e.g. if len z > len x/y...

	u64 * a = malloc(sizeof(u64)*n);
	u64 b,o_64,o_bit;

	for (int i=0; i<n; i++) {
		o_64 = z[i] >> 6;
		o_bit= z[i] - ((z[i]>>6) << 6);
		b = *(x+o_64) << o_bit;
		if (o_bit > 0) {
			b += x[o_64+1] >> (64-o_bit);
		}
		b = (b >> (64-n_y))<<(64-n_y);
		y = (y >> (64-n_y))<<(64-n_y); //not necessary, just in case
		a[i] = HammingCtr(b,y);
	}

	return a;
}

/*
int main() {
	//test hamconv
	u64 x[] = {1,2,3,4,5,6,7,8};
	u64 y = 15;
	u64 z[] = {0,64,64*2,64*3,64*4,64*5,64*6,64*7};

	u64 n_samples = sizeof(z)/sizeof(u64);
	u64 *out = ConvolutedMagic(x,y,z,n_samples,64);

	for (int i=0; i<n_samples;i++) {

	}
	return 0;
}
*/