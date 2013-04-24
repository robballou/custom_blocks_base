The Custom Blocks Base module is intended to make creating modules with blocks a bit less repetitious.

## The Original Version

The original version of this module required that module developers call all the block hooks for each module that they create and invoke the CustomBlocksManager functionality from within that. This approach is still available.

### Step 1: Implement hooks

    <?php
    /*
     * This module uses the custom_blocks_base module to easily create and manage blocks.
     *
     * To view the blocks code, see my_custom_module.blocks.inc
     */

    /**
     * Implements hook_block_info()
     */
    function my_custom_module_block_info() {
        $block_manager = _my_custom_module_block_manager();
        return $block_manager->info();
    }

    /**
     * Implements hook_block_configure
     */
    function my_custom_module_block_configure($delta = '') {
        $block_manager = _my_custom_module_block_manager();
        return $block_manager->configure($delta);
    }

    /**
     * Implements hook_block_save
     */
    function my_custom_module_block_save($delta = '', $edit = array()) {
        $block_manager = _my_custom_module_block_manager();
        return $block_manager->save($delta, $edit);
    }

    /**
     * Implements hook_block_view()
     */
    function my_custom_module_block_view($delta = '') {
        $block_manager = _my_custom_module_block_manager();
        return $block_manager->view($delta);
    }

    /**
     * Create the CustomBlocksManager for use with this module
     */
    function _my_custom_module_block_manager() {
        module_load_include('module', 'custom_blocks_base');
        module_load_include('inc', 'my_custom_module', 'my_custom_module.blocks');

        if (!isset($GLOBALS['custom_block_manager'])) {
            $GLOBALS['custom_block_manager'] = new CustomBlocksManager('MyCustomModule');
        }
        return $GLOBALS['custom_block_manager'];
    }

### Step 2: Create block classes

You may now create blocks using the provided block classes or by extending them as needed. When invoking the manager in the hooks above, we set the class prefix as "MyCustomModule", so class names should start with "MyCustomModule".

## The new version

Since it can get repetitive to implement those hooks for each module, developers can now implement `hook_custom_blocks_base_info` and return any classes that should be included. The block module hooks are now defined by as part of Custom Blocks Base.

    <?php
    function my_custom_module_custom_blocks_base_info() {
        return array(
            'MyCustomModuleBlockClass',
        );
    }

    class MyCustomModuleBlockClass extends CustomBlocksBlock {
        public function content() {
            return array('#markup' => '<p>Hello, world</p>');
        }
    }
