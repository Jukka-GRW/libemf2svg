#ifdef __cplusplus
extern "C" {
#endif

#ifndef DARWIN
#define _POSIX_C_SOURCE 200809L
#endif
#include <stdio.h>
#include "uemf.h"
#include "emf2svg_private.h"
#include "emf2svg_print.h"

void U_EMRINVERTRGN_draw(const char *contents, FILE *out,
                         drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRINVERTRGN_print(contents, states);
    }
}
void U_EMRMOVETOEX_draw(const char *contents, FILE *out,
                        drawingStates *states) {
    FLAG_SUPPORTED;
    if (states->verbose) {
        U_EMRMOVETOEX_print(contents, states);
    }

    if (states->inPath) {
        fprintf(out, "M ");
        moveto_draw("U_EMRMOVETOEX", "ptl:", "", contents, out, states);
    } else {
        PU_EMRGENERICPAIR pEmr = (PU_EMRGENERICPAIR)(contents);
        U_POINT pt = pEmr->pair;

        states->cur_x = pt.x;
        states->cur_y = pt.y;
    }
}
void U_EMRPIXELFORMAT_draw(const char *contents, FILE *out,
                           drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRPIXELFORMAT_print(contents, states);
    }
    // PU_EMRPIXELFORMAT pEmr = (PU_EMRPIXELFORMAT)(contents);
}
void U_EMRRESTOREDC_draw(const char *contents, FILE *out,
                         drawingStates *states) {
    FLAG_SUPPORTED;
    if (states->verbose) {
        U_EMRRESTOREDC_print(contents, states);
    }
    PU_EMRSETMAPMODE pEmr = (PU_EMRSETMAPMODE)(contents);
    restoreDeviceContext(states, pEmr->iMode);
}
void U_EMRSAVEDC_draw(const char *contents, FILE *out, drawingStates *states) {
    FLAG_SUPPORTED;
    if (states->verbose) {
        U_EMRSAVEDC_print(contents, states);
    }
    saveDeviceContext(states);
    UNUSED(contents);
}
void U_EMRSCALEVIEWPORTEXTEX_draw(const char *contents, FILE *out,
                                  drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRSCALEVIEWPORTEXTEX_print(contents, states);
    }
}
void U_EMRSCALEWINDOWEXTEX_draw(const char *contents, FILE *out,
                                drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRSCALEWINDOWEXTEX_print(contents, states);
    }
}
void U_EMRSETARCDIRECTION_draw(const char *contents, FILE *out,
                               drawingStates *states) {
    FLAG_SUPPORTED;
    if (states->verbose) {
        U_EMRSETARCDIRECTION_print(contents, states);
    }
    PU_EMRSETARCDIRECTION pEmr = (PU_EMRSETARCDIRECTION)contents;
    switch (pEmr->iArcDirection) {
    case U_AD_CLOCKWISE:
        states->currentDeviceContext.arcdir = 1;
        break;
    case U_AD_COUNTERCLOCKWISE:
        states->currentDeviceContext.arcdir = -1;
        break;
    }
}
void U_EMRSETBKCOLOR_draw(const char *contents, FILE *out,
                          drawingStates *states) {
    FLAG_PARTIAL;
    if (states->verbose) {
        U_EMRSETBKCOLOR_print(contents, states);
    }
    PU_EMRSETBKCOLOR pEmr = (PU_EMRSETBKCOLOR)(contents);
    states->currentDeviceContext.bk_red = pEmr->crColor.Red;
    states->currentDeviceContext.bk_blue = pEmr->crColor.Blue;
    states->currentDeviceContext.bk_green = pEmr->crColor.Green;
}
void U_EMRSETBKMODE_draw(const char *contents, FILE *out,
                         drawingStates *states) {
    FLAG_PARTIAL;
    if (states->verbose) {
        U_EMRSETBKMODE_print(contents, states);
    }
    PU_EMRSETMAPMODE pEmr = (PU_EMRSETMAPMODE)(contents);
    states->currentDeviceContext.bk_mode = pEmr->iMode;
}
void U_EMRSETBRUSHORGEX_draw(const char *contents, FILE *out,
                             drawingStates *states) {
    FLAG_UNUSED;
    if (states->verbose) {
        U_EMRSETBRUSHORGEX_print(contents, states);
    }
}
void U_EMRSETCOLORADJUSTMENT_draw(const char *contents, FILE *out,
                                  drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRSETCOLORADJUSTMENT_print(contents, states);
    }
    // PU_EMRSETCOLORADJUSTMENT pEmr = (PU_EMRSETCOLORADJUSTMENT)(contents);
}
void U_EMRSETICMMODE_draw(const char *contents, FILE *out,
                          drawingStates *states) {
    FLAG_UNUSED;
    if (states->verbose) {
        U_EMRSETICMMODE_print(contents, states);
    }
}
void U_EMRSETLAYOUT_draw(const char *contents, FILE *out,
                         drawingStates *states) {
    FLAG_PARTIAL;
    if (states->verbose) {
        U_EMRSETLAYOUT_print(contents, states);
    }
    PU_EMRSETMAPMODE pEmr = (PU_EMRSETLAYOUT)(contents);
    states->text_layout = pEmr->iMode;
}
void U_EMRSETMAPMODE_draw(const char *contents, FILE *out,
                          drawingStates *states) {
    FLAG_PARTIAL;
    PU_EMRSETMAPMODE pEmr = (PU_EMRSETMAPMODE)(contents);
    switch (pEmr->iMode) {
    case U_MM_TEXT:
        states->scalingX = states->scaling * 1;
        states->scalingY = states->scaling * 1;
        states->viewPortOrgX = 0;
        states->viewPortOrgY = 0;
        break;
    case U_MM_LOMETRIC:
        // convert to 0.1 mm to pixel and invert Y
        states->scalingX = states->scaling * states->pxPerMm * 0.1 * 1;
        states->scalingY = states->scaling * states->pxPerMm * 0.1 * -1;
        states->viewPortOrgX = 0;
        states->viewPortOrgY = states->imgHeight;
        break;
    case U_MM_HIMETRIC:
        // convert to 0.01 mm to pixel and invert Y
        states->scalingX = states->scaling * states->pxPerMm * 0.01 * 1;
        states->scalingY = states->scaling * states->pxPerMm * 0.01 * -1;
        states->viewPortOrgX = 0;
        states->viewPortOrgY = states->imgHeight;
        break;
    case U_MM_LOENGLISH:
        // convert to 0.01 inch to pixel and invert Y
        states->scalingX =
            states->scaling * states->pxPerMm * 0.01 * mmPerInch * 1;
        states->scalingY =
            states->scaling * states->pxPerMm * 0.01 * mmPerInch * -1;
        states->viewPortOrgX = 0;
        states->viewPortOrgY = states->imgHeight;
        break;
    case U_MM_HIENGLISH:
        // convert to 0.001 inch to pixel and invert Y
        states->scalingX =
            states->scaling * states->pxPerMm * 0.001 * mmPerInch * 1;
        states->scalingY =
            states->scaling * states->pxPerMm * 0.001 * mmPerInch * -1;
        states->viewPortOrgX = 0;
        states->viewPortOrgY = states->imgHeight;
        break;
    case U_MM_TWIPS:
        // convert to 1 twips to pixel and invert Y
        states->scalingX =
            states->scaling * states->pxPerMm / 1440 * mmPerInch * 1;
        states->scalingY =
            states->scaling * states->pxPerMm / 1440 * mmPerInch * -1;
        states->viewPortOrgX = 0;
        states->viewPortOrgY = states->imgHeight;
        break;
    case U_MM_ISOTROPIC:
        states->MapMode = U_MM_ISOTROPIC;
        break;
    case U_MM_ANISOTROPIC:
        states->MapMode = U_MM_ANISOTROPIC;
        break;
    }
    if (states->verbose) {
        U_EMRSETMAPMODE_print(contents, states);
    }
}
void U_EMRSETMAPPERFLAGS_draw(const char *contents, FILE *out,
                              drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRSETMAPPERFLAGS_print(contents, states);
    }
    // PU_EMRSETMAPPERFLAGS pEmr = (PU_EMRSETMAPPERFLAGS)(contents);
}
void U_EMRSETMETARGN_draw(const char *contents, FILE *out,
                          drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRSETMETARGN_print(contents, states);
    }
    UNUSED(contents);
}
void U_EMRSETMITERLIMIT_draw(const char *contents, FILE *out,
                             drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRSETMITERLIMIT_print(contents, states);
    }
}
void U_EMRSETPOLYFILLMODE_draw(const char *contents, FILE *out,
                               drawingStates *states) {
    FLAG_SUPPORTED;
    if (states->verbose) {
        U_EMRSETPOLYFILLMODE_print(contents, states);
    }
    PU_EMRSETMAPMODE pEmr = (PU_EMRSETMAPMODE)(contents);
    states->currentDeviceContext.fill_polymode = pEmr->iMode;
}
void U_EMRSETROP2_draw(const char *contents, FILE *out, drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRSETROP2_print(contents, states);
    }
}
void U_EMRSETSTRETCHBLTMODE_draw(const char *contents, FILE *out,
                                 drawingStates *states) {
    FLAG_PARTIAL;
    PU_EMRSETMAPMODE pEmr = (PU_EMRSETMAPMODE)(contents);
    states->currentDeviceContext.stretchMode = pEmr->iMode;
    if (states->verbose) {
        U_EMRSETSTRETCHBLTMODE_print(contents, states);
    }
}
void U_EMRSETTEXTALIGN_draw(const char *contents, FILE *out,
                            drawingStates *states) {
    FLAG_PARTIAL;
    if (states->verbose) {
        U_EMRSETTEXTALIGN_print(contents, states);
    }
    PU_EMRSETMAPMODE pEmr = (PU_EMRSETMAPMODE)(contents);
    states->currentDeviceContext.text_align = pEmr->iMode;
}
void U_EMRSETTEXTCOLOR_draw(const char *contents, FILE *out,
                            drawingStates *states) {
    FLAG_PARTIAL;
    if (states->verbose) {
        U_EMRSETTEXTCOLOR_print(contents, states);
    }
    PU_EMRSETTEXTCOLOR pEmr = (PU_EMRSETTEXTCOLOR)(contents);
    states->currentDeviceContext.text_red = pEmr->crColor.Red;
    states->currentDeviceContext.text_blue = pEmr->crColor.Blue;
    states->currentDeviceContext.text_green = pEmr->crColor.Green;
}
void U_EMRSETVIEWPORTEXTEX_draw(const char *contents, FILE *out,
                                drawingStates *states) {
    FLAG_PARTIAL;
    if (states->verbose) {
        U_EMRSETVIEWPORTEXTEX_print(contents, states);
    }
    PU_EMRSETVIEWPORTEXTEX pEmr = (PU_EMRSETVIEWPORTEXTEX)(contents);

    states->imgWidth = pEmr->szlExtent.cx * states->scaling;
    states->imgHeight = pEmr->szlExtent.cy * states->scaling;
}
void U_EMRSETVIEWPORTORGEX_draw(const char *contents, FILE *out,
                                drawingStates *states) {
    FLAG_IGNORED;
    if (states->verbose) {
        U_EMRSETVIEWPORTORGEX_print(contents, states);
    }
    // PU_EMRSETVIEWPORTORGEX pEmr = (PU_EMRSETVIEWPORTORGEX)(contents);
    // states->viewPortOrgX =
    //    (double)pEmr->ptlOrigin.x * states->scalingX;
    // states->viewPortOrgY =
    //    (double)pEmr->ptlOrigin.y * states->scalingY;
}
void U_EMRSETWINDOWEXTEX_draw(const char *contents, FILE *out,
                              drawingStates *states) {
    FLAG_PARTIAL;
    if (states->verbose) {
        U_EMRSETWINDOWEXTEX_print(contents, states);
    }

    PU_EMRSETWINDOWEXTEX pEmr = (PU_EMRSETVIEWPORTEXTEX)(contents);
    states->scalingX = (double)states->imgWidth / (double)pEmr->szlExtent.cx;
    states->scalingY = (double)states->imgHeight / (double)pEmr->szlExtent.cy;
    states->viewPortOrgX = states->windowOrgX;
    states->viewPortOrgY = states->windowOrgY;
}
void U_EMRSETWINDOWORGEX_draw(const char *contents, FILE *out,
                              drawingStates *states) {
    FLAG_PARTIAL;
    if (states->verbose) {
        U_EMRSETWINDOWORGEX_print(contents, states);
    }

    PU_EMRSETWINDOWORGEX pEmr = (PU_EMRSETWINDOWORGEX)(contents);
    states->viewPortOrgX =
        -1.0 * (double)pEmr->ptlOrigin.x * states->scalingX + states->viewPortOrgX;
    states->viewPortOrgY =
        -1.0 * (double)pEmr->ptlOrigin.y * states->scalingY + states->viewPortOrgY;
}

#ifdef __cplusplus
}
#endif
/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
