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

/**
 * Counts how many LevelX interfaces currently use the requested interface_type.
 * Accepts the interfaces array, or an object wrapping it.
 *
 * @param {object|Array} levelxInterfaces - interfaces array or wrapper object.
 * @param {string} expectedInterfaceType - the interface_type const to count.
 * @returns {number} number of matching interfaces.
 */
function countInterfaceType(levelxInterfaces, expectedInterfaceType) {
  if (typeof expectedInterfaceType !== "string" || expectedInterfaceType.length === 0) {
    return 0;
  }

  let array = levelxInterfaces;
  if (array && typeof array === "object" && !Array.isArray(array) && Array.isArray(array.interfaces)) {
    array = array.interfaces;
  }

  if (!Array.isArray(array)) {
    return 0;
  }

  let count = 0;
  for (let i = 0; i < array.length; i++) {
    const entry = array[i];
    const type = entry && entry.logical_config ? entry.logical_config.interface_type : undefined;
    if (type === expectedInterfaceType) {
      count++;
    }
  }

  return count;
}

module.exports = {
  isPowerOfTwo,
  isFilexNorFlashEnabled,
  isFilexNandFlashEnabled,
  countInterfaceType
};