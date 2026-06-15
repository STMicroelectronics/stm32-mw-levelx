/**
 * Check if a number is a power of two.
 *
 * @param {number} n - The number to check.
 * @returns {boolean} - Returns `true` if the number is a power of two, otherwise `false`.
 */
function isPowerOfTwo(n) {
  if (n <= 0) {
    return false;
  }
  return (n & (n - 1)) === 0;
}

/**
 * Checks if the FileX NOR Flash interface (nor_flash_itf) is enabled in the project.
 * @param {object} used_components - Components returned by SWProjectAPI.getUsedComponents
 * @returns {boolean} true if nor_flash_itf is enabled in FileX, false otherwise
 */
function isFilexNorFlashEnabled(used_components) {
  try {
    return used_components.some(
      (comp) => comp["cgroup"] === "FileX" && comp["csub"].toUpperCase() === "NOR_FLASH_ITF"
    );
  } catch (e) {
    console.error(`[ERROR] isFilexNorFlashEnabled: ${e}`);
    return false;
  }
}

/**
 * Checks if the FileX NAND Flash interface (nand_flash_itf) is enabled in the project.
 * @param {object} used_components - Components returned by SWProjectAPI.getUsedComponents
 * @returns {boolean} true if nand_flash_itf is enabled in FileX, false otherwise
 */
function isFilexNandFlashEnabled(used_components) {
  try {
    return used_components.some(
      (comp) => comp["cgroup"] === "FileX" && comp["csub"].toUpperCase() === "NAND_FLASH_ITF"
    );
  } catch (e) {
    console.error(`[ERROR] isFilexNandFlashEnabled: ${e}`);
    return false;
  }
}

module.exports = {
  isPowerOfTwo,
  isFilexNorFlashEnabled,
  isFilexNandFlashEnabled
};