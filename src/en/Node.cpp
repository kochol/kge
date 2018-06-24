#include "../../include/kge/en/Node.h"

namespace kge
{
    namespace en
    {
        //--------------------------------------------------------------------------------------------
        // Constructor
        //--------------------------------------------------------------------------------------------
        Node::Node(): m_pParent(NULL), m_eNodeType(Node::Type::Unknown)
        {

        } // Constructor

        //--------------------------------------------------------------------------------------------
        // Destructor
        //--------------------------------------------------------------------------------------------
        Node::~Node()
        {
            RemoveChildren();
            kgedecref(m_pParent);

        } // Destructor

       	//--------------------------------------------------------------------------------------------
        // Adds a node as child.
        //--------------------------------------------------------------------------------------------
        void Node::AddChild(Node* child)
        {
            m_vChilds.push_back(child);
            child->m_pParent = this;
            child->AddRef();

        } // AddChild

        //--------------------------------------------------------------------------------------------
        // Removes a child from this node.
        //--------------------------------------------------------------------------------------------
        void Node::RemoveChild(Node* child)
        {
            for(std::vector<Node*>::iterator it = m_vChilds.begin(); 
                it != m_vChilds.end(); ++it)
            {
                if ((*it) == child)
                {
                    m_vChilds.erase(it);
                    child->DecRef();
                    return;
                }
            }

        } // RemoveChild

        //--------------------------------------------------------------------------------------------
        // Removes all children of this node.
        //--------------------------------------------------------------------------------------------
        void Node::RemoveChildren()
        {
            for (std::vector<Node*>::iterator it = m_vChilds.begin();
                it != m_vChilds.end(); ++it)
            {
                (*it)->DecRef();
            }

            m_vChilds.clear();

        } // RemoveChildren

        //--------------------------------------------------------------------------------------------
        // Sets the node parent.
        //--------------------------------------------------------------------------------------------
        void Node::SetParent(Node* parent)
        {
            if (m_pParent)
            {
                m_pParent->RemoveChild(this);
                m_pParent->DecRef();
            }

            if (parent)
            {
                parent->AddChild(this);
                parent->AddRef();
            }

            m_pParent = parent;

        } // SetParent

    } // en

} // kge