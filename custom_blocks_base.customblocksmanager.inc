<?php
/**
 * Manage hook_block_x hooks
 */
class CustomBlocksManager {
  /**
   * Array of blocks found by the custom blocks manager
   */
  public $blocks = array();

  /**
   * The prefix used for class names that this manager will pick up on
   */
  public $classPrefix = '';

  /**
   * Turn on/off some dpm messages
   */
  public $debug = FALSE;

  /**
   * Create the manager and scan for classes
   *
   * @param string $classPrefix
   *   Prefix for the class
   */
  public function __construct($classPrefix) {
    $this->classPrefix = $classPrefix;

    $this->findCustomBlockClasses($this->classPrefix);
  }

  /**
   * Check if the block exists
   *
   * @param string $delta
   *   The block delta
   */
  public function blockExists($delta) {
    return isset($this->blocks[$delta]);
  }

  /**
   * Figure out the class name that will be used
   *
   * @param string $delta
   *   The delta for the block
   */
  public function className($delta) {
    return $this->classPrefix . '_' . ucwords($delta);
  }

  /**
   * Called from hook_block_configure()
   *
   * @param string $delta
   *   The block delta
   */
  public function configure($delta = '') {
    if (!$this->blockExists($delta)) {
      if ($this->debug) {
        dpm('Block does not exist: ' . $delta);
      }
      return;
    }

    $block = $this->getBlock($delta);
    if ($block && method_exists($block, 'configure') && $block->configurable === TRUE) {
      return $block->configure();
    }
  }

  /**
   * Find classes with a prefix
   */
  public function findCustomBlockClasses($prefix) {
    $classes = &drupal_static(__FUNCTION__);
    if (!$classes) {
      $classes = get_declared_classes();
    }
    foreach ($classes as $class) {
      // skip classes without our prefix
      if (substr($class, 0, strlen($prefix)) != $prefix) {
        continue;
      }

      $block = new $class();
      $this->blocks[$block->delta] = array(
        'info' => t($block->description),
        'cache' => DRUPAL_NO_CACHE,
      );
    }
  }

  /**
   * Create the block instance
   *
   * @param string $delta
   *   The block delta
   */
  public function getBlock($delta) {
    $class = $this->className($delta);
    if (class_exists($class)) {
      $block = new $class();
      return $block;
    }
    if ($this->debug) {
      dpm('Class does not exist: ' . $class);
    }
    return FALSE;
  }

  /**
   * Called from hook_block_info for this set of blocks
   */
  public function info() {
    return $this->blocks;
  }

  /**
   * Called from hook_block_save for this set of blocks
   *
   * @param string $delta
   *   The block delta
   * @param array $edit
   *   The edit info
   */
  public function save($delta, $edit) {
    if (!$this->blockExists($delta)) {
      if ($this->debug) {
        dpm('Block does not exist: ' . $delta);
      }
      return;
    }

    $block = $this->getBlock($delta);
    if ($block && method_exists($block, 'save') && $block->configurable === TRUE) {
      return $block->save($edit);
    }
  }

  /**
   * Called from hook_block_view
   *
   * @param string $delta
   *   The block delta
   */
  public function view($delta = '') {
    if (!$this->blockExists($delta)) {
      if ($this->debug) {
        dpm('Block does not exist: ' . $delta);
      }
      return;
    }

    $block = $this->getBlock($delta);
    if ($block) {
      return $block->view();
    }
  }
}

/**
 * Default class manager for even better custom blocks
 */
class CustomBlocksDefaultManager extends CustomBlocksManager {
  public $prefixes = array();

  /**
   * Constructor
   */
  public function __construct() {
    parent::__construct('CustomBlocksBase');
    $block_prefixes = custom_blocks_base_registered_blocks();
    foreach ($block_prefixes as $prefix) {
      $this->findCustomBlockClasses($prefix);
    }
  }
}
