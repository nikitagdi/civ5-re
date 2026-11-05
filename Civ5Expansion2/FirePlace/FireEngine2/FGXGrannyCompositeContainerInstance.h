/// origin guess
#pragma once

class FGXVector4;
namespace FireGrafix {
class FGXModelNode;
}

/// size ?
class FGXGrannyCompositeContainerInstance {
public:
    /// 0x08cd759c
    FGXGrannyCompositeContainerInstance();
    /// 0x08cd75ce
    ~FGXGrannyCompositeContainerInstance();
    /// 0x08cd7614
    UNK_RET Release();
    /// 0x08cd7654
    UNK_RET IsInitialized() const;
    /// 0x08cd7670
    UNK_RET surface_handles_begin();
    /// 0x08cd7678
    UNK_RET surface_handles_begin() const;
    /// 0x08cd7680
    UNK_RET surface_handles_end();
    /// 0x08cd768e
    UNK_RET get_surface_handle_count() const;
    /// 0x08cd7696
    UNK_RET surface_handles_end() const;
    /// 0x08cd76a4
    UNK_RET surface_handle_at(unsigned int a1);
    /// 0x08cd76b4
    UNK_RET surface_handle_at(unsigned int a1) const;
    /// 0x08cd76c4
    UNK_RET nodes_begin();
    /// 0x08cd76cc
    UNK_RET nodes_begin() const;
    /// 0x08cd76d4
    UNK_RET nodes_end();
    /// 0x08cd76e2
    UNK_RET get_node_count() const;
    /// 0x08cd76ea
    UNK_RET nodes_end() const;
    /// 0x08cd76f8
    UNK_RET node_at(unsigned int a1);
    /// 0x08cd7708
    UNK_RET node_at(unsigned int a1) const;
    /// 0x08cd7718
    UNK_RET transforms_begin();
    /// 0x08cd7720
    UNK_RET transforms_begin() const;
    /// 0x08cd7728
    UNK_RET transforms_end();
    /// 0x08cd773c
    UNK_RET get_template_transform_count() const;
    /// 0x08cd774e
    UNK_RET transforms_end() const;
    /// 0x08cd7762
    UNK_RET transform_at(unsigned int a1);
    /// 0x08cd7772
    UNK_RET transform_at(unsigned int a1) const;
    /// 0x08cd7782
    UNK_RET views_begin();
    /// 0x08cd778a
    UNK_RET views_begin() const;
    /// 0x08cd7792
    UNK_RET views_end();
    /// 0x08cd77a2
    UNK_RET get_template_view_count() const;
    /// 0x08cd77ae
    UNK_RET views_end() const;
    /// 0x08cd77be
    UNK_RET get_tree();
    /// 0x08cd77c6
    UNK_RET get_tree() const;
    /// 0x08cd77ce
    UNK_RET set_node_count(unsigned int a1);
    /// 0x08cd77da
    UNK_RET get_template_surface_handle_count() const;
    /// 0x08cd77ea
    UNK_RET get_template_node_count() const;
    /// 0x08cd77fc
    UNK_RET get_dag_nodes();
    /// 0x08cd7816
    UNK_RET get_dag_node_count() const;
    /// 0x08cd782e
    UNK_RET get_dag_nodes_extra_data();
    /// 0x08cd7848
    UNK_RET get_mesh_bindings();
    /// 0x08cd7862
    UNK_RET get_mesh_binding_count() const;
};

/// 0x08cd78c0
UNK_RET SetModelTransformsToIdentity(FireGrafix::FGXModelNode const* a1, unsigned int a2, FGXVector4* a3);
