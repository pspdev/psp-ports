#ifndef PSPGL_MATRIX_H
#define PSPGL_MATRIX_H

/* Write the VFPU's copy of the top-of-stack matrix back to memory */
void __pspgl_matrix_sync(struct pspgl_context *c, const struct pspgl_matrix_stack *s);

/* Load the VFPU with the top-of-stack matrix */
void __pspgl_matrix_load(struct pspgl_context *c, struct pspgl_matrix_stack *s);

/* Select a new current matrix stack, setting up the VFPU as required. */
void __pspgl_matrix_select(struct pspgl_context *c, struct pspgl_matrix_stack *s);

#endif	/* PSPGL_MATRIX_H */
