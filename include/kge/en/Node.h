#ifndef KGE_EN_NODE_H
#define KGE_EN_NODE_H

#include <vector>
#include "../KgeUnknown.h"

namespace kge
{
    namespace en
    {
        class KGE_API Node: public KgeUnknown
        {
            public:

            enum class Type
            {
                Entity = 0,
                Component,

                Unknown

            };

            //! Constructor
            Node();

            //! Destructor
            virtual ~Node();

            //! Adds a node as child.
        	virtual void AddChild(Node* child);

           	/*! Removes a child from this node.
                \param child The pointer to the child.
                \param Delete If pass true it will be deleted from memory.
            */
            virtual void RemoveChild(Node* child);

            //! Removes all children of this node.
            virtual void RemoveChildren();

            //! Returns the node parent.
            virtual const Node* GetParent() { return m_pParent; }

            //! Sets the node parent.
            virtual void SetParent(Node* parent);

            //! Returns the node type.
            Node::Type GetType() { return m_eNodeType; }

            protected:

            Node            *   m_pParent;
            std::vector<Node*>  m_vChilds;
            Node::Type          m_eNodeType;

        }; // Node

    } // en

} // kge

#endif // !KGE_EN_NODE_H
