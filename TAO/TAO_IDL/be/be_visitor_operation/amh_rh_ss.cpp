//=============================================================================
/**
*  @file   amh_rh_ss.cpp
*
*  $Id$
*
*  Creates code for AMH-RH operations.
*
*  @author Mayur Deshpande <mayur@ics.uci.edu>
*/
//=============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_visitor_operation.h"

be_visitor_amh_rh_operation_ss::be_visitor_amh_rh_operation_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_amh_rh_operation_ss::~be_visitor_amh_rh_operation_ss (void)
{
}

int
be_visitor_amh_rh_operation_ss::visit_operation (be_operation *node)
{
  // Nothing to be done for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  // Output stream.
  TAO_OutStream *os = this->ctx_->stream ();

   be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Step 1 : Generate return type: always void
  os->indent ();
  *os << "virtual void "
      << "TAO_" << intf->full_skel_name () << "::";

  // Check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }

  *os << node->local_name ();

  // Step 2 : Generate the params of the method
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_amh_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  delete visitor;

  // Step 3: Generate actual code for the method
  *os << "{" << be_idt_nl
      << "init_reply ();" << be_nl << be_nl;

  marshal_params (node);

  *os << "send_reply ();" << be_uidt_nl
      << "}" << be_nl;

  return 0;

}


int
be_visitor_amh_rh_operation_ss::marshal_params (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  // Now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled out.
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      os->indent ();

      // marshal the in and inout arguments
      *os << "if (!(\n" << be_idt;

      // Marshal each in and inout argument.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ss::"
                             "gen_demarshal_params - "
                             "codegen for demarshal failed\n"),
                            -1);
        }

      *os << be_uidt_nl << "))"
          << "ACE_THROW (CORBA::MARSHAL());" << be_nl << be_nl;
    };

  return 0;
}
