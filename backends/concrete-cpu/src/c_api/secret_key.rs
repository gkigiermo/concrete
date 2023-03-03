use super::{
    types::{Csprng, CsprngVtable},
    utils::nounwind,
};
use crate::implementation::types::{CsprngMut, LweCiphertext, LweSecretKey};

#[no_mangle]
pub unsafe extern "C" fn concrete_cpu_secret_key_size_u64(lwe_dimension: usize) -> usize {
    LweSecretKey::<&[u64]>::data_len(lwe_dimension)
}

#[no_mangle]
pub unsafe extern "C" fn concrete_cpu_init_lwe_secret_key_u64(
    lwe_sk: *mut u64,
    lwe_dimension: usize,
    csprng: *mut Csprng,
    csprng_vtable: *const CsprngVtable,
) {
    nounwind(|| {
        let csprng = CsprngMut::new(csprng, csprng_vtable);

        let sk = LweSecretKey::<&mut [u64]>::from_raw_parts(lwe_sk, lwe_dimension);

        sk.fill_with_new_key(csprng);
    })
}

#[no_mangle]
pub unsafe extern "C" fn concrete_cpu_encrypt_lwe_ciphertext_u64(
    // secret key
    lwe_sk: *const u64,
    // ciphertext
    lwe_out: *mut u64,
    // plaintext
    input: u64,
    // lwe size
    lwe_dimension: usize,
    // encryption parameters
    variance: f64,
    // csprng
    csprng: *mut Csprng,
    csprng_vtable: *const CsprngVtable,
) {
    nounwind(|| {
        let lwe_sk = LweSecretKey::from_raw_parts(lwe_sk, lwe_dimension);
        let lwe_out = LweCiphertext::from_raw_parts(lwe_out, lwe_dimension);
        lwe_sk.encrypt_lwe(
            lwe_out,
            input,
            variance,
            CsprngMut::new(csprng, csprng_vtable),
        );
    });
}

#[no_mangle]
pub unsafe extern "C" fn concrete_cpu_decrypt_lwe_ciphertext_u64(
    // secret key
    lwe_sk: *const u64,
    // ciphertext
    lwe_ct_in: *const u64,
    // lwe size
    lwe_dimension: usize,
    // plaintext
    plaintext: *mut u64,
) {
    nounwind(|| {
        let lwe_sk = LweSecretKey::from_raw_parts(lwe_sk, lwe_dimension);
        let lwe_ct_in = LweCiphertext::from_raw_parts(lwe_ct_in, lwe_dimension);
        *plaintext = lwe_sk.decrypt_lwe(lwe_ct_in);
    });
}
